#include "p2pfullnodenetwork.h"

P2PFullNodeNetwork::P2PFullNodeNetwork(QObject *parent) : QObject(parent)
{
    udp = new QUdpSocket;
    QObject::connect(udp, &QUdpSocket::readyRead, this, &P2PFullNodeNetwork::OnNetRequire);
    QObject::connect(&heartbeatTimer, &QTimer::timeout, this, &P2PFullNodeNetwork::OnHeartbeat);

    QObject::connect(&ringNet, &NP2PRingNet::Send, this, &P2PFullNodeNetwork::OnBroadcast);
}

void P2PFullNodeNetwork::Init(int Port, int heartRate)
{
    this->heartRate = heartRate;
    udp->bind(Port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    this->heartRate = heartRate;
    heartbeatTimer.start(1000 * heartRate);
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

QStringList P2PFullNodeNetwork::getNodeInfoListbyAddr(QString data)
{
    QStringList nodeInfoStringList;
    QStringList datas = data.split(',');
    foreach(QString d, datas){
        auto addr = QByteArray::fromHex(d.toLatin1());
        auto infoStr = ringNet.getNodeInfo(addr).ToString();
        nodeInfoStringList.append(infoStr);
    }
    return nodeInfoStringList;
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
        auto ret = udp->readDatagram(datagram.data(), datagram.size(),&senderIP,&senderPort);
        if(ret==-1){
            qDebug()<<udp->errorString();
            continue;
        }

        QString dataString = QString::fromLatin1(datagram);
        QIPEndPoint nat = QIPEndPoint(senderIP.toString().mid(7),senderPort);

        QString msg = "Rcv:"+ dataString + " From:"+ nat.ToString();
        qDebug()<<msg;

        MessageProtocol mp;
        auto cmd = mp.Decode(dataString);

        if(cmd == "P2PN"){//p2p neighbour nodeinfo list
            EnterMain(mp.getData(),nat);
        }

<<<<<<< HEAD
        if(cmd == "IPLS"){//addrs nodeinfo list
            QString msg = "IPLS"+getNodeInfoListbyAddr(mp.getData()).join(";");//TODO:getTotelList
            udpSend(nat,msg);
        }

        if(cmd == "ALL "){//all addrs list
            QString msg = "ALL "+getAllPeerAddrsString().join(";");//TODO:getTotelList
=======
        if(cmd == "P2P"){
            EnterMain(data,nat);
        }

        if(cmd == "ALL"){
            QString msg = "ALL"+getAllPeerAddrsString().join(";");//TODO:getTotelList
>>>>>>> 9cf25cb903b74fc3218052e8e137f6e1e7743fcb
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
<<<<<<< HEAD
    udpSend(ep,"P2PN" + msg);
=======
    udpSend(ep,"P2P" + msg);
>>>>>>> 9cf25cb903b74fc3218052e8e137f6e1e7743fcb
}

qint64 P2PFullNodeNetwork::udpSend(QIPEndPoint endPoint, QString msg)
{
    auto ret = udp->writeDatagram(msg.toLatin1(),endPoint.IP(),endPoint.Port());
    if(ret==-1){
        qDebug()<<udp->errorString();
    }
    return ret;
}
