#include "p2pfullnodenetwork.h"

P2PFullNodeNetwork::P2PFullNodeNetwork(QObject *parent) : QObject(parent)
{
    udp = new QUdpSocket;
    connect(udp, &QUdpSocket::readyRead, this,&P2PFullNodeNetwork::OnNetinRequire);
}

void P2PFullNodeNetwork::Init(int Port, int heartRate)
{
    this->heartRate = heartRate;
    udp->bind(Port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void P2PFullNodeNetwork::BoardCast(SubNet net)
{
    auto nodes = net.getMemberList();
    auto msg = ("P2P"+net.GetMemberList()).toLatin1();
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
    return mainNetwork.GetMemberList().split(';');
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
        QString netID = datas[3];
        if(netID == QString("0")){
            mainNetwork.Enter(dataString);
            peers.insert(QString(datas[0]),nat);
            qDebug()<<datas[0];
            qDebug()<<peers[QString(datas[0])].IP();
            BoardCast(mainNetwork);
            emit NewConnect();
        }else{
            //TODO: enter subnet
        }
    }
}
