#include "nclientinterface.h"

NClientInterface::NClientInterface(QObject *parent) : QObject(parent)
{
    SetPort(StartPort);
    //p2p.Init();
    QObject::connect(&ipcCause,&UdpIPC::Rcv,this,&NClientInterface::OnRcvLocalCause,Qt::QueuedConnection);
    QObject::connect(&ipcResult,&UdpIPC::Rcv,this,&NClientInterface::OnRcvLocalResult,Qt::QueuedConnection);
}

void NClientInterface::SetPort(int port)
{
    ipcCause.SetPort(port);
    ipcResult.SetPort(port+2);
}

void NClientInterface::SendCause(QString cause)
{
    ipcCause.Send(cause);
}

void NClientInterface::SendResult(QString result)
{
    ipcResult.Send(result);
}

void NClientInterface::OnRcvLocalCause(QString msg)
{
    qDebug()<<__FUNCTION__<<msg;
    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1());
    quint64 ts = obj["ts"].toDouble();
    quint64 data = obj["data"].toString();
    consensus.RcvCause(ts,p2p.localAddr(),data);
}

void NClientInterface::OnRcvLocalResult(QString msg)
{
    qDebug()<<__FUNCTION__<<msg;
    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1());
    quint64 ts = obj["ts"].toDouble();
    quint64 data = obj["data"].toString();
    consensus.RcvResult(ts,p2p.localAddr(),data);
}
