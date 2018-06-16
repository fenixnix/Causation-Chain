#include "p2pfullnodenetwork.h"

P2PFullNodeNetwork::P2PFullNodeNetwork(QObject *parent) : QObject(parent)
{
    udp = new QUdpSocket;
    connect(udp, &QUdpSocket::readyRead, this,&P2PFullNodeNetwork::OnNetinRequire);
}

void P2PFullNodeNetwork::Init(QString address, int Port, int heartRate)
{
    this->heartRate = heartRate;
    udp->bind(QHostAddress(address),Port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void P2PFullNodeNetwork::BoardCast(SubNet net)
{
    auto nodes = net.getMemberList();
    auto msg = ("P2P"+net.GetMemberList()).toLatin1();
    foreach (auto n, nodes) {
        //udp->writeDatagram(net.GetMemberList().toLatin1(),n.nat.IP(),n.nat.Port());
        qDebug()<<__FUNCTION__<<n.loc.IP()<<n.loc.Port();
        auto ret = udp->writeDatagram(msg,n.loc.IP(),n.loc.Port());
        if(ret==-1){
            qDebug()<<udp->errorString();
        }
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
        QIPEndPoint nat = QIPEndPoint(senderIP,senderPort);

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
            BoardCast(mainNetwork);
            //udp->writeDatagram(mainNetwork.GetMemberList().toLatin1(),senderIP,senderPort);
            emit NewConnect();
        }else{
            //TODO: enter subnet
        }
    }
}
