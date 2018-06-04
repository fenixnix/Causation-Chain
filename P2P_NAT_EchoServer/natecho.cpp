#include "natecho.h"
#include <iostream>

using namespace std;

NatEcho::NatEcho(QObject *parent) : QObject(parent)
{
  udp = new QUdpSocket;
  udp->bind(UDPPORT,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
  connect(udp, &QUdpSocket::readyRead, this,&NatEcho::OnUDP);
}

void NatEcho::OnUDP()
{
  while(udp->hasPendingDatagrams())
    {
      QByteArray datagram;
      QHostAddress senderIP;
      quint16 senderPort;
      datagram.resize(udp->pendingDatagramSize());
      udp->readDatagram(datagram.data(), datagram.size(),&senderIP,&senderPort);

      QString nat = senderIP.toString().mid(7) + ":" + QString::number(senderPort);
      udp->writeDatagram(nat.toLatin1(),senderIP,senderPort);

      QString msg = "Rcv:"+ QString::fromLatin1(datagram) +
          " From:"+ nat;
      cout<<msg.toStdString()<<endl;
    }
}
