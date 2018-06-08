#include "p2pfullnodenetwork.h"

P2PFullNodeNetwork::P2PFullNodeNetwork(QObject *parent) : QObject(parent)
{
  udp = new QUdpSocket;
  udp->bind(portNetinRequire,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
  connect(udp, &QUdpSocket::readyRead, this,&P2PFullNodeNetwork::OnNetinRequire);
}

void P2PFullNodeNetwork::BoardCast(SubNet net)
{
  auto nodes = net.getMemberList();
  foreach (auto n, nodes) {
      udp->writeDatagram(net.GetMemberList().toLatin1(),n.nat.IP(),n.nat.Port());
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
      QString hash = datas[4];
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
