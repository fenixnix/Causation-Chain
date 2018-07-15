#include "np2pnode.h"
#include "ipclassify.h"
#define CMDSIZE 4

NP2PNode::NP2PNode(QObject *parent) : QObject(parent)
{
    udpP2p = new QUdpSocket;
    udpNat = new QUdpSocket;
    QObject::connect(udpP2p, &QUdpSocket::readyRead, this, &NP2PNode::OnP2PServer);
    QObject::connect(udpNat, &QUdpSocket::readyRead, this, &NP2PNode::OnNat);

    QObject::connect(&heartbeatTimer, &QTimer::timeout, this, &NP2PNode::OnHeartbeat);
}

NP2PNode::~NP2PNode()
{
    heartbeatTimer.stop();
    udpP2p->close();
    udpNat->close();
    delete udpP2p;
    delete udpNat;
}

void NP2PNode::setID(QString id)
{
    this->localAddress = id;
}

void NP2PNode::bindLocalEndPoint(QIPEndPoint localEndPoint)
{
    udpNat->close();
    udpNat->bind(localEndPoint.IP(),localEndPoint.Port(),
                 QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    //qDebug()<<udpNat->localAddress()<<udpNat->localPort();
}

void NP2PNode::setP2PServer(QIPEndPoint server)
{
    this->p2pServer.Init(server.IP().toString(),server.Port());
    bindP2PServer(QIPEndPoint(udpNat->localAddress(),server.Port()));
}

void NP2PNode::bindP2PServer(QIPEndPoint p2pServer)
{
    udpP2p->close();
    udpP2p->bind(p2pServer.IP(),p2pServer.Port(),
                 QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void NP2PNode::join(QIPEndPoint endPoint)
{
    if(endPoint.IP() == udpNat->localAddress()){
        natEndPoint.Init(endPoint.IP().toString(),udpNat->localPort());
        RequireJoin();
    }else{
        udpNatSend(endPoint, localAddress);
    }

    heartbeatTimer.start(HeartBeatInterval*1000);
}

QStringList NP2PNode::neighbourList()
{
    auto ls = net.getMemberList().keys();
    ls.append(localAddress);
    return ls;
}

void NP2PNode::Query(QString msg)
{
    udpSend(p2pServer,msg);
}

void NP2PNode::RequireJoin()
{
    QString msg = MessageProtocol::Encode("P2PN",localAddress
            + "," + QIPEndPoint(udpNat->localAddress(),udpNat->localPort()).ToString()
            + "," + natEndPoint.ToString());
    Query(msg);
}

void NP2PNode::RequireAllPeersList()
{
    QString msg = "ALL ";
    Query(msg);
}

void NP2PNode::RequireNatbyAddr(QByteArrayList addrs)
{
    QStringList ls;
    foreach(auto a, addrs){
        ls.append(QString(a.toHex()));
    }
    QString msg = "IPLS" + ls.join(',');
    Query(msg);
}

qint64 NP2PNode::udpNatSend(QIPEndPoint endPoint, QString msg)
{
    auto ret = udpNat->writeDatagram(msg.toLatin1(),endPoint.IP(),endPoint.Port());
    if(ret==-1){
        qDebug()<<udpNat->errorString();
    }
    return ret;
}

void NP2PNode::sendbyAddr(QString msg, QString id)
{
    if(this->localAddress == id){
        //qDebug()<<"Can`t send to myself!"<<this->id<<id;
        return;
    }

    if(!net.has(id)){
        //qDebug()<<id<<" not in net";
        return;
    }

    auto data = msg;
    auto nodeInfo = net.get(id);

    if(nodeInfo.nat.IP() == natEndPoint.IP()){
        //LAN
        //qDebug()<<"LAN P2P:"<<data<<" to:"<<nodeInfo.loc.ToString();
        udpNatSend(nodeInfo.loc, data);
    }else{
        //WAN
        //qDebug()<<"WAN P2P:"<<data<<" to:"<<nodeInfo.nat.ToString();
        udpNatSend(nodeInfo.nat, data);
    }
}

void NP2PNode::sendMsg(QString msg, QString id)
{
    sendbyAddr("MSG "+msg,id);
}

void NP2PNode::broadcastMsg(QString msg)
{
    auto ls = neighbourList();
    foreach(auto m ,ls){
        sendMsg(msg,m);
    }
}

qint64 NP2PNode::udpSend(QIPEndPoint endPoint, QString msg)
{
    auto ret = udpP2p->writeDatagram(msg.toLatin1(),endPoint.IP(),endPoint.Port());
    if(ret==-1){
        qDebug()<<udpP2p->errorString();
    }
    return ret;
}

QHostAddress NP2PNode::getLocalIP()
{
    auto hostName = QHostInfo::localHostName();
    auto host = QHostInfo::fromName(hostName);
    //qDebug()<<host.addresses();
    foreach(auto ip, host.addresses()){
        if(ip.protocol() == QAbstractSocket::IPv4Protocol){
            return ip;
        }
    }
    return QHostAddress();
}

QString NP2PNode::getLocalIP2(){
    QString ipAddr;
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    for(QHostAddress address:AddressList){
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            if (address.toString().contains("127.0.")){
                continue;
            }
            return address.toString();
        }
    }
    return ipAddr;
}

void NP2PNode::OnP2PServer()
{
    qDebug()<<__FUNCTION__;
    while(udpP2p->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpP2p->pendingDatagramSize());
        auto ret = udpP2p->readDatagram(datagram.data(), datagram.size());
        if(ret==-1){
            qDebug()<<udpP2p->errorString();
            continue;
        }
        auto msg = QString::fromLatin1(datagram);

        MessageProtocol mp;
        auto cmd = mp.Decode(msg);
        if(cmd == "P2PN"){
            qDebug()<<"Rcv P2P:"+ mp.getData();
            GetP2PList(mp.getData());
        }

        if(cmd == "ALL "){
            qDebug()<<"Rcv All Addr:"+ mp.getData();
            GetAllAddr(mp.getData());
        }

        if(cmd == "IPLS"){
            qDebug()<<"Rcv NAT by Addr:" + mp.getData();
            GetNatbyAddr(mp.getData());
        }
    }
}

void NP2PNode::OnNat()
{
    while(udpNat->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress senderIP;
        quint16 senderPort;
        datagram.resize(udpNat->pendingDatagramSize());
        auto ret = udpNat->readDatagram(datagram.data(), datagram.size(),&senderIP,&senderPort);
        if(ret==-1){
            qDebug()<<udpNat->errorString();
            continue;
        }
        auto msg = QString::fromLatin1(datagram);
        //qDebug()<<__FUNCTION__<<": "<<msg;

        MessageProtocol mp;
        auto cmd = mp.Decode(msg);

        if(cmd == "NAT "){
            natEndPoint.Init(mp.getData());
            //qDebug()<<"Rcv NAT:"+ natEndPoint.ToString();
            RequireJoin();
        }
        if(cmd == "HTBT"){
            //qDebug()<<"HB from:"<<data;
            net.heartbeat(mp.getData());
        }
        if(cmd == "MSG "){
            //qDebug()<<"Message: "<<data;
            emit RcvMsg(mp.getData());
        }

        if(cmd == "PING"){

        }

        if(cmd == "PONG"){

        }
    }
}

void NP2PNode::OnHeartbeat()
{
    RequireJoin();
    net.removeDeadMemberAtNow();
    foreach(auto memberID, neighbourList()){
        sendbyAddr("HTBT" + localAddress,memberID);
    }
    emit neighbourListUpdate(neighbourList());
}

void NP2PNode::GetP2PList(QString data)
{
    auto datas = data.split(';');
    foreach (auto d, datas) {
        NodeInfo info;
        info.SetData(d);
        //skip myself
//        if(info.getId() == this->localAddress){
//            continue;
//        }
        net.enter(d);
    }
    emit neighbourListUpdate(neighbourList());
}

void NP2PNode::GetAllAddr(QString data)
{
    emit RcvP2PAllAddress(data.split(';'));
}

void NP2PNode::GetNatbyAddr(QString data)
{
    QStringList ls = data.split(";");
}
