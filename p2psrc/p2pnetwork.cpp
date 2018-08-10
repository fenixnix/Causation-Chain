#include "p2pnetwork.h"

P2PNetwork::P2PNetwork(QObject *parent) : QObject(parent)
{
    connect(&udp, &UdpNetwork::Rcv, this, &P2PNetwork::OnRcv);
    connect(&heartbeatTimer, &QTimer::timeout, this, &P2PNetwork::OnHeartbeat,Qt::QueuedConnection);
    connect(&ringNet, &NP2PRingNet::Send, this, &P2PNetwork::OnBroadcast,Qt::QueuedConnection);
}

void P2PNetwork::Init(int Port, int heartRate)
{
    this->heartRate = heartRate;
    udp.Listen(Port);
    heartbeatTimer.start(1000 * heartRate);
}

void P2PNetwork::Join(QString data, QIPEndPoint nat)
{
    NPeerData info;
    if(!info.SetData(data)) return;

    ringNet.peerJoinCall(info);
    peers.insert(info.addrID,nat);
}

QStringList P2PNetwork::getNodeInfoListbyAddr(QString data)
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

QByteArrayList P2PNetwork::getAllPeerAddrs()
{
    return ringNet.getAllAddress();
}

QStringList P2PNetwork::getAllPeerAddrsString()
{
    auto brl = getAllPeerAddrs();
    QStringList list;
    foreach(auto p, brl){
        list.append(QString(p.toHex()));
    }
    return list;
}

void P2PNetwork::OnRcv(QString msg, QHostAddress ip, quint16 port)
{
    MessageProtocol mp;
    auto cmd = mp.Decode(msg);
    auto senderEP = QIPEndPoint(ip,port);

    if(cmd == "P2PN"){//p2p neighbour nodeinfo list
        Join(mp.getData(),senderEP);
    }

    if(cmd == "IPLS"){//addrs nodeinfo list
        QString msg = "IPLS"+getNodeInfoListbyAddr(mp.getData()).join(";");//TODO:getTotelList
        udp.Send(senderEP,msg);
    }

    if(cmd == "ALL "){//all addrs list
        QString msg = "ALL "+getAllPeerAddrsString().join(";");//TODO:getTotelList
        udp.Send(senderEP,msg);
    }
}

void P2PNetwork::OnHeartbeat()
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

void P2PNetwork::OnBroadcast(QByteArray addr, QIPEndPoint endPoint, QString msg)
{
    auto ep = peers[addr];
    udp.Send(ep,"P2PN" + msg);
}
