#include "p2pfullnodenetwork.h"

P2PFullNodeNetwork::P2PFullNodeNetwork(QObject *parent) : QObject(parent)
{
    udp = new QUdpSocket;
    QObject::connect(udp, &QUdpSocket::readyRead, this,&P2PFullNodeNetwork::OnNetinRequire);
    QObject::connect(&heartbeatTimer, &QTimer::timeout, this, &P2PFullNodeNetwork::OnHeartbeat);
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
    if(datas.size()<4){
        return;
    }
    mainNet.enter(data);
    peers.insert(QString(datas[0]),nat);
    qDebug()<<datas[0];
    qDebug()<<peers[QString(datas[0])].IP();
    BoardCast(mainNet);
    emit UpdateMemberList();
}

void P2PFullNodeNetwork::BoardCast(NSubNet net)
{
    auto nodes = net.getMemberList();
    auto msg = ("P2P"+net.getMemberListString()).toLatin1();
    foreach (auto n, nodes) {
        auto endPoint = &peers[n.id];
        auto ret = udp->writeDatagram(msg,endPoint->IP(),endPoint->Port());
        if(ret==-1){
            qDebug()<<udp->errorString();
        }
        qDebug()<<__FUNCTION__<<n.id<<endPoint->IP()<<endPoint->Port();
    }
}

QStringList P2PFullNodeNetwork::GetMainNetwrokNodes()
{
    return mainNet.getMemberListString().split(';');
}

void P2PFullNodeNetwork::OnNetinRequire()
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

        auto datas = dataString.split(',');
        //      QString hash = datas[4];
        if(datas.size()<4){
            continue;
        }
        if(datas[3] == QString("0")){//main net
            EnterMain(dataString,nat);
//            mainNet.enter(dataString);
//            peers.insert(QString(datas[0]),nat);
//            qDebug()<<datas[0];
//            qDebug()<<peers[QString(datas[0])].IP();
//            BoardCast(mainNet);
//            emit UpdateMemberList();
        }else{
            //TODO: enter subnet
        }
    }
}

void P2PFullNodeNetwork::OnHeartbeat()
{
    qDebug()<<__FUNCTION__;
    mainNet.removeDeadMemberAtNow();
    QStringList deadList;
    foreach(auto p, peers.keys()){
        if(!mainNet.has(p)){
            deadList.append(p);
        }
    }
    foreach(auto d, deadList){
        qDebug()<<"dead"<<d;
        peers.remove(d);
    }
    emit UpdateMemberList();
}
