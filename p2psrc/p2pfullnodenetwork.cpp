#include "p2pfullnodenetwork.h"

P2PFullNodeNetwork::P2PFullNodeNetwork(QObject *parent) : QObject(parent)
{
    udp = new QUdpSocket;
    QObject::connect(udp, &QUdpSocket::readyRead, this,&P2PFullNodeNetwork::OnNetRequire);
    QObject::connect(&heartbeatTimer, &QTimer::timeout, this, &P2PFullNodeNetwork::OnHeartbeat);

    QObject::connect(&ringNet, &NP2PRingNet::Send, this, &P2PFullNodeNetwork::OnBroadcast);
}

void P2PFullNodeNetwork::Init(int Port, int heartRate)
{
    this->heartRate = heartRate;
    udp->bind(Port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    this->heartRate = heartRate;
    heartbeatTimer.start(1000*heartRate);
}

void P2PFullNodeNetwork::EnterMain(QString data, QIPEndPoint nat)
{
    auto datas = data.split(',');
    if(datas.size()<3){
        return;
    }

    NodeInfo info;
    info.SetData(data);
    ringNet.peerJoinCall(info);
    peers.insert(info.addr,nat);
}

QByteArrayList P2PFullNodeNetwork::getAllPeerAddrs()
{
    return ringNet.getAllAddress();
}

QStringList P2PFullNodeNetwork::getAllPeerAddrsString()
{
    auto brl = getAllPeerAddrs();
    QStringList list;
    foreach(auto p, brl){
        list.append(QString(p.toHex()));
    }
    return list;
}

void P2PFullNodeNetwork::OnNetRequire()
{
    while(udp->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress senderIP;
        quint16 senderPort;
        datagram.resize(udp->pendingDatagramSize());
        udp->readDatagram(datagram.data(), datagram.size(),&senderIP,&senderPort);

        QString dataString = QString::fromLatin1(datagram);
        QIPEndPoint nat = QIPEndPoint(senderIP.toString().mid(7),senderPort);

        QString msg = "Rcv:"+ dataString + " From:"+ nat.ToString();
        qDebug()<<msg;

        if(dataString.size()<CMDSIZE) continue;
        auto cmd = dataString.left(CMDSIZE);
        auto data = dataString.mid(CMDSIZE);

        if(cmd == "P2P"){
            EnterMain(data,nat);
        }

        if(cmd == "ALL"){
            QString msg = "ALL"+getAllPeerAddrsString().join(";");//TODO:getTotelList
            udpSend(nat,msg);
        }
    }
}

void P2PFullNodeNetwork::OnHeartbeat()
{
    qDebug()<<__FUNCTION__;
    ringNet.update();
    QByteArrayList deadList;
    foreach(auto p, peers.keys()){
        if(!ringNet.getAllAddress().contains(p)){
            deadList.append(p);
        }
    }
    foreach(auto d, deadList){
        peers.remove(d);
    }
}

void P2PFullNodeNetwork::OnBroadcast(QByteArray addr, QIPEndPoint endPoint, QString msg)
{
    auto ep = peers[addr];
    udpSend(ep,"P2P" + msg);
}

qint64 P2PFullNodeNetwork::udpSend(QIPEndPoint endPoint, QString msg)
{
    auto ret = udp->writeDatagram(msg.toLatin1(),endPoint.IP(),endPoint.Port());
    if(ret==-1){
        qDebug()<<udp->errorString();
    }
    return ret;
}
