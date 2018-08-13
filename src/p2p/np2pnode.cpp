#include "np2pnode.h"
#include "ipclassify.h"
#define CMDSIZE 4

NP2PNode::NP2PNode(QObject *parent) : QObject(parent)
{
    QObject::connect(&nat, &UdpNetwork::Rcv, this, &NP2PNode::OnNatMsg);
    QObject::connect(&heartbeatTimer, &QTimer::timeout, this, &NP2PNode::OnHeartbeat);
}

NP2PNode::~NP2PNode()
{
    heartbeatTimer.stop();
}

void NP2PNode::Init(QString id, QIPEndPoint natServer, QIPEndPoint local)
{
    setID(id);
    bindLocalEndPoint(local);
    join(natServer);
}

void NP2PNode::SetP2PList(QString data)
{
    auto datas = data.split(';');
    foreach (QString d, datas) {
        NPeerData info;
        info.SetData(d);
        //skip myself
        if(info.getId() == this->ID){
            continue;
        }
        net.enter(d);
    }
}

void NP2PNode::AddPeerJson(QString jsonString)
{
    net.enter(NPeerData(jsonString));
}

void NP2PNode::AddPeerJson(QJsonObject json)
{
    net.enter(NPeerData(json));
}

void NP2PNode::setID(QString id)
{
    this->ID = id;
}

QString NP2PNode::getID() const
{
    return ID;
}

void NP2PNode::bindLocalEndPoint(QIPEndPoint localEndPoint)
{
    nat.Listen(localEndPoint.IP(),localEndPoint.Port());
}

void NP2PNode::join(QIPEndPoint ep)
{
    //    if(endPoint.IP() == nat.getRcvAddr()){
    //        natEndPoint.Init(endPoint.IP().toString(),nat.getSendPort());
    //        RequireJoin();
    //    }else{
    qDebug()<<__FUNCTION__<<__LINE__<<ep.ToString();
    natServer.Init(ep.ToString());
    nat.Send(natServer.IP(),natServer.Port(),ID);
    //    }

    heartbeatTimer.start(HeartBeatInterval*1000);
}

QStringList NP2PNode::neighbourList()
{
    auto ls = net.getMemberList().keys();
    //ls.append(localAddress);
    return ls;
}

QIPEndPoint NP2PNode::getLocalEndPoint()
{
    return nat.getRcvEndPoint();
}

QHostAddress NP2PNode::getLocalAddress()
{
    return nat.getRcvAddr();
}

quint16 NP2PNode::getLocalPort()
{
    return nat.getRcvPort();
}

void NP2PNode::sendbyAddr(QString msg, QString id)
{
    if(this->ID == id){
        //qDebug()<<"Can`t send to myself!"<<this->id<<id;
        return;
    }

    if(!net.has(id)){
        //qDebug()<<id<<" not in net";
        return;
    }

    auto data = msg;
    auto nodeInfo = net.get(id);

    //qDebug()<<__FUNCTION__<<natEndPoint.ToString();
    if(nodeInfo.nat.IP() == natEndPoint.IP()){
        //LAN
        //qDebug()<<"LAN P2P:"<<data<<" to:"<<nodeInfo.loc.ToString();
        nat.Send(nodeInfo.loc.IP(),nodeInfo.loc.Port(),data);
    }else{
        //WAN
        //qDebug()<<"WAN P2P:"<<data<<" to:"<<nodeInfo.nat.ToString();
        nat.Send(nodeInfo.nat.IP(),nodeInfo.nat.Port(),data);
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

void NP2PNode::OnNatMsg(QString msg)
{
    MessageProtocol mp;
    auto cmd = mp.Decode(msg);

    //qDebug()<<__FUNCTION__<<"CMD: "<<cmd<<"MSG:"<<msg;

    if(cmd == "NAT "){
        natEndPoint.Init(mp.getData());
        //qDebug()<<"Rcv NAT:"+ natEndPoint.ToString();
        emit RequireJoin();
        return;
    }

    if(cmd == "MSG "){
        //qDebug()<<"Message: "<<mp.getData();
        emit RcvMsg(mp.getData());
        return;
    }

    if(cmd == "PING"){
        //qDebug()<<"Ping from:"<<mp.getData();
        auto addr = mp.getData();
        net.rcvPing(addr);
        sendbyAddr("PONG"+ID,addr);
        return;
    }

    if(cmd == "PONG"){
        //qDebug()<<"Pong from:"<<mp.getData();
        Pong(mp.getData());
        return;
    }
    qDebug()<<"Undefine CMD:"<<cmd;
}

void NP2PNode::OnHeartbeat()
{
    emit RequireJoin();
    //qDebug()<<__FUNCTION__<<__LINE__<<neighbourList();
    net.removeDeadMemberAtNow();
    foreach(auto memberID, neighbourList()){
        Ping(memberID);
    }
    emit neighbourListUpdate(neighbourList());

    nat.Send(natServer.IP(),natServer.Port(),ID);//heartBeat to nat server
}

void NP2PNode::Ping(QString addr)
{
    net.ping(addr);
    sendbyAddr("PING" + ID,addr);
}

void NP2PNode::Pong(QString addr)
{
    net.pong(addr);
}

QIPEndPoint NP2PNode::getNatEndPoint() const
{
    return natEndPoint;
}

QString NP2PNode::getLocalInfoString()
{
    return getID() + "," + getLocalEndPoint().ToString() + "," + getNatEndPoint().ToString();
}
