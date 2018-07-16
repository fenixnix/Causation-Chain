#include "nclientinterface.h"

NClientInterface::NClientInterface(QObject *parent) : QObject(parent)
{
    fromCCausePort = StartPort;
    toCCausePort = StartPort + 1;
    fromCResultPort = StartPort + 2;
    toCResultPort = StartPort + 3;
    p2p.Init();

    QObject::connect(&udpCause,&QUdpSocket::readyRead,this,&NClientInterface::OnRcvCause,Qt::QueuedConnection);
    QObject::connect(&udpResult,&QUdpSocket::readyRead,this,&NClientInterface::OnRcvResult,Qt::QueuedConnection);
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
        auto res = udpCause.readDatagram(datagram.data(), datagram.size());
        if(res == -1){
            qDebug()<<udpCause.errorString();
            continue;
        }
        auto msg = QString::fromLatin1(datagram);
        qDebug()<<__FUNCTION__<<msg;
        emit RcvCause(msg);
    }
}

void NClientInterface::OnRcvResult()
{
    while(udpResult.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpResult.pendingDatagramSize());
        auto res = udpResult.readDatagram(datagram.data(), datagram.size());
        if(res == -1){
            qDebug()<<udpResult.errorString();
            continue;
        }
        auto msg = QString::fromLatin1(datagram);
        qDebug()<<__FUNCTION__<<msg;
        emit RcvResult(msg);
        auto hash = QCryptographicHash::hash(msg.toLatin1(),QCryptographicHash::Keccak_256);
        qDebug()<<__FUNCTION__<<QString(hash.toHex());
        emit RcvResultHash(hash);
    }
}
