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
      auto ret = udp->readDatagram(datagram.data(), datagram.size(),&senderIP,&senderPort);
      if(ret==-1){
          qDebug()<<udp->errorString();
          continue;
      }

      QString nat = senderIP.toString().mid(7) + ":" + QString::number(senderPort);
      QString sendCmd = "NAT "+nat;
      ret = udp->writeDatagram(sendCmd.toLatin1(),senderIP,senderPort);
      if(ret==-1){
          qDebug()<<udp->errorString();
      }

      QString msg = "Rcv:"+ QString::fromLatin1(datagram) +
          " From:"+ nat;
      cout<<msg.toStdString()<<endl;
    }
}
