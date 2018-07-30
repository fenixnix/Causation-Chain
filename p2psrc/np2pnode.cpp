#include "np2pnode.h"
#include "ipclassify.h"
#define CMDSIZE 4

NP2PNode::NP2PNode(QObject *parent) : QObject(parent)
{
    QObject::connect(&p2pServerInterface, &NP2PServerInterface::ServerMsg, this, &NP2PNode::OnP2PMsg);
    QObject::connect(&nat, &UdpNetwork::Rcv, this, &NP2PNode::OnNatMsg);
    QObject::connect(&heartbeatTimer, &QTimer::timeout, this, &NP2PNode::OnHeartbeat);
}

NP2PNode::~NP2PNode()
{
    heartbeatTimer.stop();
}

void NP2PNode::Init(QString id, QIPEndPoint natServer, QIPEndPoint p2pServer, QIPEndPoint local)
{
    setID(id);
    bindLocalEndPoint(local);
    setP2PServer(p2pServer);
    join(natServer);
}

void NP2PNode::SetP2PList(QString data)
{
    auto datas = data.split(';');
    foreach (auto d, datas) {
        NodeInfo info;
        info.SetData(d);
        //skip myself
        if(info.getId() == this->ID){
            continue;
        }
        net.enter(d);
    }
}

void NP2PNode::setID(QString id)
{
    this->ID = id;
}

QString NP2PNode::getID()
{
    return ID;
}

void NP2PNode::bindLocalEndPoint(QIPEndPoint localEndPoint)
{
    nat.Listen(localEndPoint.IP(),localEndPoint.Port());
}

void NP2PNode::setP2PServer(QIPEndPoint server)
{
    p2pServerInterface.Init(server.IP(),server.Port());
}

void NP2PNode::join(QIPEndPoint endPoint)
{
    if(endPoint.IP() == nat.getRcvAddr()){
        natEndPoint.Init(endPoint.IP().toString(),nat.getSendPort());
        RequireJoin();
    }else{
        nat.Send(endPoint.IP(),endPoint.Port(),ID);
    }

    heartbeatTimer.start(HeartBeatInterval*1000);
}

QStringList NP2PNode::neighbourList()
{
    auto ls = net.getMemberList().keys();
    //ls.append(localAddress);
    return ls;
}

void NP2PNode::RequireJoin()
{
    QString msg = MessageProtocol::Encode("P2PN",getLocalInfoString());
    p2pServerInterface.Query(msg);
}

void NP2PNode::RequireAllPeersList()
{
    QString msg = "ALL ";
    p2pServerInterface.Query(msg);
}

void NP2PNode::RequireNatbyAddr(QByteArrayList addrs)
{
    QStringList ls;
    foreach(auto a, addrs){
        ls.append(QString(a.toHex()));
    }
    QString msg = "IPLS" + ls.join(',');
    p2pServerInterface.Query(msg);
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

void NP2PNode::OnP2PMsg(QString cmd, QString dat)
{
    if(cmd == "P2PN"){
        qDebug()<<"Rcv P2P:"+ dat;
        GetP2PList(dat);
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
        RequireJoin();
        return;
    }

    if(cmd == "MSG "){
        //qDebug()<<"Message: "<<data;
        emit RcvMsg(mp.getData());
        return;
    }

    if(cmd == "PING"){
        //qDebug()<<"Ping from:"<<mp.getData();
        sendbyAddr("PONG"+ID,mp.getData());
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
    RequireJoin();
    net.removeDeadMemberAtNow();
    foreach(auto memberID, neighbourList()){
        Ping(memberID);
    }
    emit neighbourListUpdate(neighbourList());
}

void NP2PNode::GetP2PList(QString data)
{
    SetP2PList(data);
    emit neighbourListUpdate(neighbourList());
}

//void NP2PNode::GetAllAddr(QString data)
//{
//    emit RcvP2PAllAddress(data.split(';'));
//}

void NP2PNode::Ping(QString addr)
{
    sendbyAddr("PING" + ID,addr);
    net.ping(addr);
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
