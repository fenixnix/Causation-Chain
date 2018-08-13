#include "udpnetwork.h"

UdpNetwork::UdpNetwork(QObject *parent) : QObject(parent)
{
    connect(&udp,&QUdpSocket::readyRead,this,&UdpNetwork::OnRcv);
}

UdpNetwork::~UdpNetwork()
{
    udp.close();
}

void UdpNetwork::Listen(quint16 port)
{
    Listen(QHostAddress::LocalHost,port);
}

void UdpNetwork::Listen(QIPEndPoint endPoint)
{
    Listen(endPoint.IP(),endPoint.Port());
}

void UdpNetwork::Listen(QHostAddress addr, quint16 port)
{
    qDebug()<<__FUNCTION__<<__LINE__<<addr<<port;
    udp.close();
    udp.bind(addr,port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void UdpNetwork::SetSendEndPoint(QHostAddress addr, quint16 port)
{
    sendAddr = addr;
    sendPort = port;
}

void UdpNetwork::SetIPCPort(int port)
{
    Listen(QHostAddress::LocalHost,port);
    SetSendEndPoint(QHostAddress::LocalHost,port+1);
}

void UdpNetwork::Close()
{
    udp.close();
}

bool UdpNetwork::Send(QString msg)
{
    //qDebug()<<__FUNCTION__<<msg;
    auto res = udp.writeDatagram(msg.toLatin1(),sendAddr,sendPort);
    if(res == -1){
        qDebug()<<udp.errorString();
        return false;
    }
    return true;
}

bool UdpNetwork::Send(QHostAddress addr, quint16 port, QString msg)
{
    auto res = udp.writeDatagram(msg.toLatin1(),addr,port);
    if(res == -1){
        qDebug()<<udp.errorString();
        return false;
    }
    return true;
}

bool UdpNetwork::Send(QIPEndPoint ep, QString msg)
{
    return Send(ep.IP(), ep.Port(), msg);
}

bool UdpNetwork::SendAndSet(QHostAddress addr, quint16 port, QString msg)
{
    SetSendEndPoint(addr,port);
    return Send(msg);
}

void UdpNetwork::OnRcv()
{
    while(udp.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udp.pendingDatagramSize());
        QHostAddress senderIP;
        quint16 senderPort;
        auto res = udp.readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);
        if(res == -1){
            qDebug()<<udp.errorString();
            continue;
        }
        auto msg = QString::fromLatin1(datagram);
        //qDebug()<<__FUNCTION__<<msg;
        emit Rcv(msg,senderIP,senderPort);
    }
}

quint16 UdpNetwork::getSendPort() const
{
    return sendPort;
}

QHostAddress UdpNetwork::getRcvAddr() const
{
    return udp.localAddress();
}

quint16 UdpNetwork::getRcvPort() const
{
    return udp.localPort();
}

QIPEndPoint UdpNetwork::getRcvEndPoint() const
{
    return QIPEndPoint(udp.localAddress(),udp.localPort());
}

QHostAddress UdpNetwork::getSendAddr() const
{
    return sendAddr;
}
