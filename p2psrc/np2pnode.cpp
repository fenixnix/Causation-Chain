#include "np2pnode.h"
#include "ipclassify.h"

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
    return net.getMemberList().keys();
}

void NP2PNode::Query(QString msg)
{
    udpSend(p2pServer,msg);
}

void NP2PNode::RequireJoin()
{
    QString msg = localAddress + "," + QIPEndPoint(udpNat->localAddress(),udpNat->localPort()).ToString()
            + "," + natEndPoint.ToString() + "," + "0";
//    QString msg = "JOIN" + localAddress + "," + QIPEndPoint(udpNat->localAddress(),udpNat->localPort()).ToString()
//            + "," + natEndPoint.ToString() + "," + "0";
    Query(msg);
}

void NP2PNode::RequireAllPeersList()
{
    QString msg = "ALLP";
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

void NP2PNode::sendbyID(QString msg, QString id)
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
    sendbyID("MSG"+msg,id);
}

void NP2PNode::boardcastMsg(QString msg)
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
    //qDebug()<<__FUNCTION__;
    while(udpP2p->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpP2p->pendingDatagramSize());
        udpP2p->readDatagram(datagram.data(), datagram.size());
        auto str = QString::fromLatin1(datagram);
        QString msg = "Rcv:"+ str +
                " From P2PServer";
        //qDebug()<<__FUNCTION__<<msg;
        auto cmd = str.left(3);
        auto data = str.mid(3);

        if(cmd == "P2P"){
            //qDebug()<<"Rcv P2P:"+ data;
            GetP2PList(data);
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
        udpNat->readDatagram(datagram.data(), datagram.size(),&senderIP,&senderPort);
        auto str = QString::fromLatin1(datagram);
        qDebug()<<__FUNCTION__<<": "<<str;
        auto cmd = str.left(3);
        auto data = str.mid(3);
        if(cmd == "NAT"){
            natEndPoint.Init(data);
            //qDebug()<<"Rcv NAT:"+ natEndPoint.ToString();
            RequireJoin();
        }
        if(cmd == "HB "){
            //qDebug()<<"HB from:"<<data;
            net.heartbeat(data);
        }
        if(cmd == "MSG"){
            //qDebug()<<"Message: "<<data;
            emit RcvMsg(data);
        }

        if(cmd == "PIN"){
            //qDebug()<<"Message: "<<data;

        }

        if(cmd == "PON"){

        }
    }
}

void NP2PNode::OnHeartbeat()
{
    RequireJoin();
    net.removeDeadMemberAtNow();
    foreach(auto memberID, neighbourList()){
        sendbyID("HB " + localAddress,memberID);
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
        if(info.getId() == this->localAddress){
            continue;
        }
        net.enter(d);
    }
    emit neighbourListUpdate(neighbourList());
}
