#include "np2pnode.h"
#include "ipclassify.h"

NP2PNode::NP2PNode(QObject *parent) : QObject(parent)
{
    udpP2p = new QUdpSocket;
    udpNat = new QUdpSocket;
    QObject::connect(udpP2p, &QUdpSocket::readyRead, this, &NP2PNode::OnP2PServer);
    QObject::connect(udpNat, &QUdpSocket::readyRead, this,&NP2PNode::OnNat);

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
    this->id = id;
}

void NP2PNode::bindLocalEndPoint(QIPEndPoint localEndPoint)
{
    udpNat->close();
    udpNat->bind(localEndPoint.IP(),localEndPoint.Port(),
                 QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    qDebug()<<udpNat->localAddress()<<udpNat->localPort();
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
        RequireEnterP2PNetwork();
    }else{
        udpNat->writeDatagram(id.toLatin1(),endPoint.IP(),endPoint.Port());
    }

    heartbeatTimer.start(HeartBeatInterval*1000);
}

QStringList NP2PNode::memberList()
{
    return net.getMemberList().keys();
}

void NP2PNode::Query(QString msg)
{
    qDebug()<<"Send:"<<msg<<" To:"<<p2pServer.ToString();
    udpP2p->writeDatagram(msg.toLatin1(),p2pServer.IP(),p2pServer.Port());
}

void NP2PNode::RequireEnterP2PNetwork()
{
    QString msg = id + "," + QIPEndPoint(udpNat->localAddress(),udpNat->localPort()).ToString()
            + "," + natEndPoint.ToString() + "," + "0";
    Query(msg);
}

void NP2PNode::SendbyEndPoint(QString msg, QIPEndPoint endPoint)
{
    udpNat->writeDatagram(msg.toLatin1(),endPoint.IP(),endPoint.Port());
}

void NP2PNode::sendbyID(QString msg, QString id)
{
    if(this->id == id){
        qDebug()<<"Can`t send to myself!"<<this->id<<id;
        return;
    }

    if(!net.has(id)){
        qDebug()<<id<<" not in net";
        return;
    }

    auto data = msg;
    auto nodeInfo = net.get(id);

    if(nodeInfo.nat.IP() == natEndPoint.IP()){
        //LAN
        qDebug()<<"LAN P2P:"<<data<<" to:"<<nodeInfo.loc.ToString();
        SendbyEndPoint(data, nodeInfo.loc);
    }else{
        //WAN
        qDebug()<<"WAN P2P:"<<data<<" to:"<<nodeInfo.nat.ToString();
        SendbyEndPoint(data, nodeInfo.nat);
    }
}

void NP2PNode::sendMsg(QString msg, QString id)
{
    sendbyID("MSG"+msg,id);
}

void NP2PNode::boardcastMsg(QString msg)
{
    auto ls = memberList();
    foreach(auto m ,ls){
        sendMsg(msg,m);
    }
}

QHostAddress NP2PNode::getLocalIP()
{
    auto hostName = QHostInfo::localHostName();
    auto host = QHostInfo::fromName(hostName);
    qDebug()<<host.addresses();
    foreach(auto ip, host.addresses()){
        if(ip.protocol() == QAbstractSocket::IPv4Protocol){
            return ip;
        }
    }
    return QHostAddress();
}

void NP2PNode::OnP2PServer()
{
    qDebug()<<__FUNCTION__;
    while(udpP2p->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpP2p->pendingDatagramSize());
        udpP2p->readDatagram(datagram.data(), datagram.size());
        auto str = QString::fromLatin1(datagram);
        QString msg = "Rcv:"+ str +
                " From P2PServer";
        qDebug()<<msg;
        auto cmd = str.left(3);
        auto data = str.mid(3);

        if(cmd == "P2P"){
            qDebug()<<"Rcv P2P:"+ data;
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
            qDebug()<<"Rcv NAT:"+ natEndPoint.ToString();
            RequireEnterP2PNetwork();
        }
        if(cmd == "HB "){
            qDebug()<<"HB from:"<<data;
            net.heartbeat(data);
        }
        if(cmd == "MSG"){
            qDebug()<<"Message: "<<data;
            emit RcvMsg(data);
        }
    }
}

void NP2PNode::OnHeartbeat()
{
    RequireEnterP2PNetwork();
    net.removeDeadMemberAtNow();
    foreach(auto memberID, memberList()){
        sendbyID("HB " + id,memberID);
    }
    emit P2PmemberListUpdate(memberList());
}

void NP2PNode::GetP2PList(QString data)
{
    auto datas = data.split(';');
    foreach (auto d, datas) {
        NodeInfo info;
        info.SetData(d);
        if(info.id == this->id){
            continue;
        }
        net.enter(d);
    }
    emit P2PmemberListUpdate(memberList());
}
