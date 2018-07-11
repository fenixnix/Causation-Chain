#include "nclientinterface.h"

NClientInterface::NClientInterface(QObject *parent) : QObject(parent)
{
    fromCCausePort = StartPort;
    toCCausePort = StartPort + 1;
    fromCResultPort = StartPort + 2;
    toCResultPort = StartPort + 3;
    p2p.Init();

    QObject::connect(&udpCause,&QUdpSocket::readyRead,this,&NClientInterface::OnRcvCause);
    QObject::connect(&udpResult,&QUdpSocket::readyRead,this,&NClientInterface::OnRcvResult);
    udpCause.bind(fromCCausePort,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    udpResult.bind(fromCResultPort,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void NClientInterface::SetPort(int port)
{
    fromCCausePort = port;
    toCCausePort = port + 1;
    fromCResultPort = port + 2;
    toCResultPort = port + 3;
}

void NClientInterface::SendCause(QString cause)
{
    udpCause.writeDatagram(cause.toLatin1(),QHostAddress::LocalHost,toCCausePort);
}

void NClientInterface::SendResult(QString result)
{
    udpResult.writeDatagram(result.toLatin1(),QHostAddress::LocalHost,toCResultPort);
}

void NClientInterface::OnRcvCause()
{
    while(udpCause.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpCause.pendingDatagramSize());
        udpCause.readDatagram(datagram.data(), datagram.size());
        auto msg = QString::fromLatin1(datagram);
        qDebug()<<msg;
        emit RcvCause(msg);
    }
}

void NClientInterface::OnRcvResult()
{
    while(udpResult.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpResult.pendingDatagramSize());
        udpResult.readDatagram(datagram.data(), datagram.size());
        auto msg = QString::fromLatin1(datagram);
        emit RcvResult(msg);
    }
}
