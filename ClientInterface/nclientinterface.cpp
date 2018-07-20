#include "nclientinterface.h"

NClientInterface::NClientInterface(QObject *parent) : QObject(parent)
{
    QObject::connect(&timeSync, &NTimeSync::Tick, this, &NClientInterface::OnTick);

    SetPort(StartPort);
    QObject::connect(&ipc, &UdpIPC::Rcv, this, &NClientInterface::OnRcvLocal);

    QObject::connect(&p2p, &NCryptoP2P::RcvMsg, this, &NClientInterface::OnRcvNet);
    p2p.Init();

    timeOut.setSingleShot(true);
    QObject::connect(&timeOut, &QTimer::timeout, this, &NClientInterface::OnCauseTimeOut);
}

void NClientInterface::SetPort(int port)
{
    ipc.SetPort(port);
}

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
    if(cmd == "FIN"){
        //TODO:report result to main network
    }
}

void NClientInterface::OnRcvNet(quint64 timeStamp, QString addr, QString msg)
{
    auto obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    auto frame = obj["frame"].toInt();
    auto cmd = obj["cmd"].toString();
    if(cmd == "cause"){
        RcvNetCause(frame,addr,obj["data"].toString());
    }
    if(cmd == "causePack"){
        RcvNetCausePack(frame,addr,obj["data"].toString());
    }
    if(cmd == "result"){
        RcvNetResult(frame,addr,obj["data"].toString());
    }
}

void NClientInterface::RcvLocalCause(QString data)
{
    //2.获取到当前帧收集到的本地控制命令，广播
    QJsonObject obj = QJsonDocument::fromJson(data.toLatin1()).object();
    quint64 frame = obj["frame"].toDouble();
    QString dataString = obj["data"].toString();
    packer.Push(p2p.localAddr(),dataString);
    timeOut.start(500);

    QJsonObject sobj;
    sobj.insert("frame",frame);
    sobj.insert("addr",p2p.localAddr());
    sobj.insert("cmd","cause");
    sobj.insert("data",dataString);
    p2p.boardcastMsg(QString(QJsonDocument(sobj).toJson()));//广播本地控制命令
}

void NClientInterface::RcvNetCause(quint64 frame, QString addr, QString data)
{
    //3.1收集其他节点的控制命令，在收集全部参与者的控制命令
    packer.Push(addr,data);
    if(packer.Size()==articipators.size()){
        BroadcastCause();
    }
}

void NClientInterface::OnCauseTimeOut()
{
    //3.2收集参与者控制命令 超时
    BroadcastCause();
}

void NClientInterface::BroadcastCause()
{
    //4.广播接收到的打包操作命令
    timeOut.stop();
    auto jsonDat = packer.PackJsonString();
    consensus.RcvCause(frame,p2p.localAddr(),jsonDat);//本地操作命令参与共识
    QJsonObject obj;
    obj.insert("frame",frame);
    obj.insert("addr",p2p.localAddr());
    obj.insert("cmd","causePark");
    obj.insert("data",jsonDat);
    p2p.boardcastMsg(QString(QJsonDocument(obj).toJson()));
}

void NClientInterface::RcvNetCausePack(quint64 frame, QString addr, QString data)
{
    //5.共识输入达成50%以上时执行。
    consensus.RcvCause(frame,addr,data);
    //TODO: 输入共识：if(consensus)
    //ipc.Send("CAU",consensus.ReachCauseConsensus(););
}

void NClientInterface::RcvLocalResult(QString data)
{
    //6.接收本地执行结果，并广播
    packer.Clear();//删除控制命令
    qDebug()<<__FUNCTION__<<data;
    QJsonObject obj = QJsonDocument::fromJson(data.toLatin1()).object();
    quint64 frame = obj["frame"].toDouble();
    QString dataString = obj["data"].toString();
    consensus.RcvResult(frame, p2p.localAddr(),dataString);

    QJsonObject sobj;
    sobj.insert("frame",frame);
    sobj.insert("addr",p2p.localAddr());
    sobj.insert("cmd","result");
    sobj.insert("data",dataString);
    p2p.boardcastMsg(QString(QJsonDocument(sobj).toJson()));//广播本地结果
}

void NClientInterface::RcvNetResult(quint64 frame, QString addr, QString data)
{
    //7.接收网络执行结果，共识，并推入区块,甄别作弊者
    consensus.RcvResult(frame,addr,data);
}
