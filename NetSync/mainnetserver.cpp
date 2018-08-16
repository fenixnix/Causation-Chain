#include "mainnetserver.h"
#include "npeerdata.h"
#include "messageprotocol.h"

MainNetServer::MainNetServer(QObject *parent) : QObject(parent)
{
    connect(&interface, &NP2PServerInterface::ServerMsg, this, &MainNetServer::OnServerMsg);
    connect(&p2p, &NP2PNode::RcvMsg, this, &MainNetServer::OnP2pMsg);
    connect(&p2p, &NP2PNode::RequireJoin, this, &MainNetServer::OnJoin);
    Init();
}

void MainNetServer::Init(QString addrID)
{
    QSettings p2pSetting("p2p.cfg",QSettings::IniFormat);
    QIPEndPoint local(p2pSetting.value("Local").toString());
    QIPEndPoint natServer(p2pSetting.value("NATServer").toString());
    QIPEndPoint p2pServer(p2pSetting.value("P2PServer").toString());
    qDebug()<<__FUNCTION__<<natServer.ToString()<<p2pServer.ToString();
    interface.Init(local,p2pServer);
    p2p.Init(addrID,natServer,local);
}

void MainNetServer::Init()
{
    auto id = QCryptographicHash::hash(QTime::currentTime().toString().toLatin1(),QCryptographicHash::Sha256);
    Init(QString(id).left(8));
}

QString MainNetServer::getID()
{
    return p2p.getID();
}

void MainNetServer::RequireJoin()
{
    QString msg = MessageProtocol::Encode("P2PN",p2p.getLocalInfoString());
    interface.Query(msg);
}

void MainNetServer::RequireFullNode()
{
    QString msg = "ALL ";
    interface.Query(msg);
}

QStringList MainNetServer::getNeighbourList()
{
    return p2p.neighbourList();
}

void MainNetServer::BroadcastMsg(QString msg)
{
    p2p.broadcastMsg(msg);
}

void MainNetServer::SendMsg(QString msg, QString addr)
{
    p2p.sendMsg(msg, addr);
}

//void NP2PNode::RequireNatbyAddr(QByteArrayList addrs)
//{
//    QStringList ls;
//    foreach(auto a, addrs){
//        ls.append(QString(a.toHex()));
//    }
//    QString msg = "IPLS" + ls.join(',');
//    interface.Query(msg);
//}

void MainNetServer::OnServerMsg(QString cmd, QString dat)
{
    if(cmd == "P2PN"){
        //qDebug()<<"Rcv P2P:"+ dat;
        UpdateP2PList(dat);
    }

    if(cmd == "ALL "){
        //qDebug()<<"Rcv All Addr:"+ dat;
        //GetAllAddr(dat);
    }

    if(cmd == "IPLS"){
        //qDebug()<<"Rcv NAT by Addr:" + dat;
        //GetNatbyAddr(dat);
    }
}

void MainNetServer::OnP2pMsg(QString msg)
{
    emit RcvMsg(msg);
}

void MainNetServer::OnJoin()
{
    RequireJoin();
}

void MainNetServer::UpdateP2PList(QString data)
{
    p2p.SetP2PList(data);
    emit P2PListUpdate(p2p.neighbourList());
}
