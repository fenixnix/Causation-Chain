#include "udpipc.h"

UdpIPC::UdpIPC(QObject *parent) : QObject(parent)
{

}

void UdpIPC::SetPort(int port)
{
    rcvPort = port;
    sendPort = port+1;
    QObject::connect(&udp,&QUdpSocket::readyRead,
                     this,&UdpIPC::OnRcv);
    udp.bind(rcvPort,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void UdpIPC::Send(QString msg)
{
    auto res = udp.writeDatagram(msg.toLatin1(),QHostAddress::LocalHost,sendPort);
    if(res == -1){
        qDebug()<<udp.errorString();
    }
}

void UdpIPC::OnRcv()
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
