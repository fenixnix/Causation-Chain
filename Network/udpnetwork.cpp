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

void UdpNetwork::OnRcv()
{
    while(udp.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udp.pendingDatagramSize());
        auto res = udp.readDatagram(datagram.data(), datagram.size());
        if(res == -1){
            qDebug()<<udp.errorString();
            continue;
        }
        auto msg = QString::fromLatin1(datagram);
        emit Rcv(msg);
    }
}
