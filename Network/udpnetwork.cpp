#include "udpnetwork.h"

UdpNetwork::UdpNetwork(QObject *parent) : QObject(parent)
{

}

void UdpNetwork::Listen(QHostAddress addr, quint16 port)
{
    QObject::connect(&udp,&QUdpSocket::readyRead,this,&UdpNetwork::OnRcv);
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

bool UdpNetwork::Send(QString msg)
{
    auto res = udp.writeDatagram(msg.toLatin1(),QHostAddress::LocalHost,sendPort);
    //auto res = udp.writeDatagram(msg.toLatin1(),QHostAddress("192.168.1.145"),sendPort);
    if(res == -1){
        qDebug()<<udp.errorString();
        return false;
    }
    return true;
}

bool UdpNetwork::Send(QHostAddress addr, quint16 port, QString msg)
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
        emit Rcv(msg,senderIP,senderPort);
    }
}
