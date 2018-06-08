#include "np2pnode.h"

NP2PNode::NP2PNode(QObject *parent) : QObject(parent)
{
  udp = new QUdpSocket;
  udp->bind(8421,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
  QObject::connect(udp, &QUdpSocket::readyRead, this, &NP2PNode::OnUDP);
  udpNat = new QUdpSocket;

  QObject::connect(&heartbeatTimer, &QTimer::timeout, this, &NP2PNode::OnHeartbeat);
  heartbeatTimer.start(HeartBeatInterval*1000);
}

NP2PNode::~NP2PNode()
{
  udp->close();
  udpNat->close();
  delete udp;
  delete udpNat;
}

void NP2PNode::RequireNatIpEndPoint(QIPEndPoint endPoint)
{
  udpNat->bind(endPoint.Port(),QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
  connect(udpNat, &QUdpSocket::readyRead, this,&NP2PNode::OnNat);
  udpNat->writeDatagram(GetLocalAddress().toString().toLatin1(),
                     endPoint.IP(),endPoint.Port());
}

QHostAddress NP2PNode::GetLocalAddress()
{
  QHostAddress addr;
  QString localHostName = QHostInfo::localHostName();
  QHostInfo info = QHostInfo::fromName(localHostName);
  foreach(QHostAddress address, info.addresses())
    {
      if (address.protocol() == QAbstractSocket::IPv4Protocol){
          //qDebug() << "IPv4 Address:" << address.toString();
          return address;
        }
    }
  return addr;
}

void NP2PNode::SelfTest()
{
  qDebug()<<GetLocalAddress();
  RequireNatIpEndPoint(QIPEndPoint(QHostAddress("192.168.100.117"),8888));
}

void NP2PNode::SendbyEndPoint(QString msg, QIPEndPoint endPoint)
{
  udpNat->writeDatagram(msg.toLatin1(),endPoint.IP(),endPoint.Port());
}

void NP2PNode::OnUDP()
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
      //cout<<msg.toStdString()<<endl;
    }
}

void NP2PNode::OnNat()
{
  while(udpNat->hasPendingDatagrams())
    {
      QByteArray datagram;
      QHostAddress senderIP;
      quint16 senderPort;
      datagram.resize(udpNat->pendingDatagramSize());
      udpNat->readDatagram(datagram.data(), datagram.size(),&senderIP,&senderPort);
      natEndPoint.Init(QString::fromLatin1(datagram));
      qDebug()<<"Rcv NAT:"+ natEndPoint.ToString();
    }
}

void NP2PNode::OnHeartbeat()
{
  foreach(auto member, p2pMemberList){
      if(member.id!=id){
          if(member.nat.IP() == natEndPoint.IP()){
              //LAN P2P
            }else{
              //WAN P2P

            }
        }
    }
}
