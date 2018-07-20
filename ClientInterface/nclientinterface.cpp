#include "nclientinterface.h"

NClientInterface::NClientInterface(QObject *parent) : QObject(parent)
{
    QObject::connect(&timeSync, &NTimeSync::Tick, this, &NClientInterface::OnTick);

    SetPort(StartPort);
    QObject::connect(&ipc, &UdpIPC::Rcv, this, &NClientInterface::OnRcvLocal);

    QObject::connect(&p2p, &NCryptoP2P::RcvMsg, this, &NClientInterface::OnRcvNetCause);
    QObject::connect(&p2p, &NCryptoP2P::RcvMsg, this, &NClientInterface::OnRcvNetResult);
    p2p.Init();

    timeOut.setSingleShot(true);
    QObject::connect(&timeOut, &QTimer::timeout, this, &NClientInterface::OnCauseTimeOut);
}

void NClientInterface::SetPort(int port)
{
    ipc.SetPort(port);
    //ipcResult.SetPort(port+2);
}

//void NClientInterface::SendCause(QString cause)
//{
//    ipc.Send(cause);
//}

//void NClientInterface::SendResult(QString result)
//{
//    ipc.Send(result);
//}

void NClientInterface::OnTick(int frameNo)
{
    //1.向客户端请求本地控制命令；
    QJsonObject obj;
    obj.insert("frame",frameNo);
    ipc.Send("REQ"+QString(QJsonDocument(obj).toJson()));
}

#define CMDSIZE 3
void NClientInterface::OnRcvLocal(QString msg)
{
    if(msg.size()<CMDSIZE){
        return;
    }
    auto cmd = msg.left(CMDSIZE);
    if(cmd == "CAU"){
        auto data = msg.mid(CMDSIZE);
        RcvLocalCause(data);
    }
    if(cmd == "RES"){
        auto data = msg.mid(CMDSIZE);
        RcvLocalResult(data);
    }
}

void NClientInterface::OnRcvNetCause(quint64 frame, QString addr, QString data)
{
    //3.1收集其他节点的控制命令，在收集全部参与者的控制命令
    consensus.RcvCause(frame,addr,data);//将本地结果也推入控制命令共识
    //BroadcastCause();
}

void NClientInterface::OnRcvNetResult(quint64 frame, QString addr, QString data)
{

}

void NClientInterface::RcvLocalCause(QString data)
{
    //2.获取到当前帧收集到的本地控制命令，广播
    p2p.boardcastMsg(data);//广播本地控制命令
    QJsonObject obj = QJsonDocument::fromJson(data.toLatin1()).object();
    quint64 frame = obj["frame"].toDouble();
    QString data = obj["data"].toString();
    consensus.RcvCause(frame,p2p.localAddr(),data);//将本地结果也推入控制命令共识
    //TODO:设置接收超时定时器
    timeOut.start(500);
}

void NClientInterface::RcvLocalResult(QString data)
{
    //5.接收本地执行结果，并广播
    qDebug()<<__FUNCTION__<<msg;
    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    quint64 ts = obj["frame"].toDouble();
    QString data = obj["data"].toString();
    //consensus.RcvResult(ts,p2p.localAddr(),data);
}

void NClientInterface::OnCauseTimeOut()
{
    //3.2收集参与者控制命令后超时
    //BroadcastCause();
}

void NClientInterface::BroadcastCause(QString addr, QString causeString)
{
    timeOut.stop();
    //4.共识输入达成50%以上时执行。
    ipc.Send(causeString);
}
