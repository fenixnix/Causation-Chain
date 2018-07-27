#include "mainnetserver.h"

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
        GetP2PList(dat);
    }

    if(cmd == "ALL "){
        qDebug()<<"Rcv All Addr:"+ dat;
        GetAllAddr(dat);
    }

    if(cmd == "IPLS"){
        qDebug()<<"Rcv NAT by Addr:" + dat;
        GetNatbyAddr(dat);
    }
}

void MainNetServer::UpdateP2PList(QString data)
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
