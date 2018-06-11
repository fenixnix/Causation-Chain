#include "np2pnode.h"

NP2PNode::NP2PNode(QObject *parent) : QObject(parent)
{
  id = QHostInfo::localHostName();
  p2pServer.Init("192.168.100.204",9999);

  udpP2p = new QUdpSocket;
  udpNat = new QUdpSocket;
  QObject::connect(udpP2p, &QUdpSocket::readyRead, this, &NP2PNode::OnP2PServer);
  QObject::connect(udpNat, &QUdpSocket::readyRead, this,&NP2PNode::OnNat);

  QObject::connect(&heartbeatTimer, &QTimer::timeout, this, &NP2PNode::OnHeartbeat);
  heartbeatTimer.start(HeartBeatInterval*1000);
}

NP2PNode::~NP2PNode()
{
  udpP2p->close();
  udpNat->close();
  delete udpP2p;
  delete udpNat;
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

void NP2PNode::setID(QString id)
{
  this->id = id;
}

void NP2PNode::BindLocalEndPoint(QIPEndPoint localEndPoint)
{
  udpNat->close();
  udpNat->bind(localEndPoint.IP(),localEndPoint.Port(),
               QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
  qDebug()<<udpNat->localAddress()<<udpNat->localPort();
}

void NP2PNode::BindP2PServer(QIPEndPoint p2pServer)
{
  udpP2p->close();
  udpP2p->bind(p2pServer.IP(),p2pServer.Port(),
               QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void NP2PNode::RequireNAT(QIPEndPoint endPoint)
{
  udpNat->writeDatagram(id.toLatin1(),endPoint.IP(),endPoint.Port());
}

void NP2PNode::RequireEnterP2PNetwork()
{
  QString msg = id + "," + QIPEndPoint(udpNat->localAddress(),udpNat->localPort()).ToString()
      + "," + natEndPoint.ToString() + "," + "0";
  Query(msg);
}

void NP2PNode::Query(QString msg)
{
  qDebug()<<"Send:"<<msg<<" To:"<<p2pServer.ToString();
  udpP2p->writeDatagram(msg.toLatin1(),p2pServer.IP(),p2pServer.Port());
}

void NP2PNode::SelfTest()
{
  //qDebug()<<GetLocalAddress();
  BindLocalEndPoint(QIPEndPoint(QHostAddress("192.168.100.201"),8421));
  BindP2PServer(QIPEndPoint(QHostAddress("192.168.100.201"),9999));
  RequireNAT(QIPEndPoint(QHostAddress("118.178.127.35"),8888));
}

void NP2PNode::SendbyEndPoint(QString msg, QIPEndPoint endPoint)
{
  udpNat->writeDatagram(msg.toLatin1(),endPoint.IP(),endPoint.Port());
}

void NP2PNode::SendbyID(QString msg, QString id)
{
  if(this->id == id){
      return;
    }
  auto nodeInfo = p2pMemberList[id];
  if(nodeInfo.nat.IP() == udpNat->localAddress()){
      //LAN
      SendbyEndPoint(msg, nodeInfo.loc);
    }else{
      //WAN
      SendbyEndPoint(msg, nodeInfo.nat);
    }
}

bool NP2PNode::CheckAlivebyID(QString id)
{
  return p2pMemberList[id].CheckAlive();
}

void NP2PNode::OnP2PServer()
{
  while(udpP2p->hasPendingDatagrams())
    {
      QByteArray datagram;
      datagram.resize(udpP2p->pendingDatagramSize());
      udpP2p->readDatagram(datagram.data(), datagram.size());
      QString msg = "Rcv:"+ QString::fromLatin1(datagram) +
          " From P2PServer";
      qDebug()<<msg;
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
      qDebug()<<"Message: "<<QString(datagram);
      auto cmd = QString::fromLatin1(datagram).left(3);
      auto data = QString::fromLatin1(datagram).mid(3);
      if(cmd == "NAT"){
          natEndPoint.Init(data);
          qDebug()<<"Rcv NAT:"+ natEndPoint.ToString();
          RequireEnterP2PNetwork();
        }
      if(cmd == "HB "){
          p2pMemberList[data].HeartBeat();
        }
      if(cmd == "MSG"){

        }
    }
}

void NP2PNode::OnHeartbeat()
{
  qDebug()<<"Send HeartBeat";
  QStringList deadList;
  foreach(auto memberID, p2pMemberList.keys()){
      SendbyID(id,memberID);
      if(CheckAlivebyID(memberID)){
          deadList.append(memberID);
        }
    }
  //Del Dead Peer;
  foreach (auto d, deadList) {
      p2pMemberList.remove(d);
    }
}
