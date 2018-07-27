#include "mainnetserver.h"
#include "nodeinfo.h"

MainNetServer::MainNetServer(QObject *parent) : QObject(parent)
{
    Init();
}

void MainNetServer::Init()
{
    QObject::connect(&interface, &NP2PServerInterface::ServerMsg, this, &MainNetServer::OnServerMsg);
}

void MainNetServer::OnServerMsg(QString cmd, QString dat)
{
    if(cmd == "P2PN"){
        qDebug()<<"Rcv P2P:"+ dat;
        UpdateP2PList(dat);
    }

    if(cmd == "ALL "){
        qDebug()<<"Rcv All Addr:"+ dat;
        GetFullNode(dat);
    }

    if(cmd == "IPLS"){
        qDebug()<<"Rcv NAT by Addr:" + dat;
        //GetNatbyAddr(dat);
    }
}

void MainNetServer::UpdateP2PList(QString data)
{
    auto datas = data.split(';');
    foreach (auto d, datas) {
        NodeInfo info;
        info.SetData(d);
        //skip myself
//        if(info.getId() == this->localAddress){
//            continue;
//        }
        //TODO:
        //net.enter(d);
    }
    //TODO:
    emit P2PListUpdate(datas);
}

void MainNetServer::GetFullNode(QString data)
{
    emit FullNodeUpdate(data.split(';'));
}
