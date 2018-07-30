#include "mainnetserver.h"
#include "nodeinfo.h"
#include "messageprotocol.h"

MainNetServer::MainNetServer(QObject *parent) : QObject(parent)
{
    QObject::connect(&interface, &NP2PServerInterface::ServerMsg, this, &MainNetServer::OnServerMsg);
    Init();
}

void MainNetServer::Init()
{

}

void MainNetServer::RequireJoin()
{
    QString msg = MessageProtocol::Encode("P2PN",p2p.getLocalInfoString());
    interface.Query(msg);
}

void MainNetServer::RequireFullNode()
{

}

void MainNetServer::OnServerMsg(QString cmd, QString dat)
{
    if(cmd == "P2PN"){
        qDebug()<<"Rcv P2P:"+ dat;
        UpdateP2PList(dat);
    }

    if(cmd == "ALL "){
        //        qDebug()<<"Rcv All Addr:"+ dat;
        //        GetAllAddr(dat);
    }

    if(cmd == "IPLS"){
        //qDebug()<<"Rcv NAT by Addr:" + dat;
        //GetNatbyAddr(dat);
    }
}

void MainNetServer::UpdateP2PList(QString data)
{
    p2p.SetP2PList(data);
    emit P2PListUpdate(p2p.neighbourList());
}
