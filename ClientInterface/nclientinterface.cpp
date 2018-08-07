#include "nclientinterface.h"
#include "wtccmddefine.h"

#define TEST

NClientInterface::NClientInterface(QObject *parent) : QObject(parent)
{
    connect(&timeSync, &NTimeSync::Tick, this, &NClientInterface::OnTick);
    connect(&ipc, &UdpNetwork::Rcv, this, &NClientInterface::OnRcvLocal);
    connect(&p2p, &NP2PNode::RequireJoin, this, &NClientInterface::OnReadyJoin);
    connect(&p2p, &NP2PNode::RcvMsg, this, &NClientInterface::OnRcvP2P);
    connect(&server, &NP2PServerInterface::ServerMsg, this, &NClientInterface::OnRcvServerMsg);
    connect(&timeOut, &QTimer::timeout, this, &NClientInterface::OnCauseTimeOut);

#ifdef TEST
    connect(&onn, &OnnConnector::RcvMsg, this, &NClientInterface::OnOnnMsg);
#endif
    Init();
}

void NClientInterface::Init()
{
    const QString cryptoFileName = "crypto.cfg";
    QSettings cryptoSetting(cryptoFileName, QSettings::IniFormat);
    if(!QFile(cryptoFileName).exists()){
        qDebug()<<"Not find crypto.cfg, generate new keyPair!!";
        NEmcc ecc;
        ecc.GenerateKeyPair();
        cryptoSetting.setValue("SecKey", ecc.privateKeyString);
        cryptoSetting.setValue("PubKey", ecc.publicKeyString);
        cryptoSetting.sync();
    }

    QString secKey = cryptoSetting.value("SecKey").toString();
    QString pubKey = cryptoSetting.value("PubKey").toString();
    Init(secKey, pubKey);

}

void NClientInterface::Init(QString secKey, QString pubKey)
{
    //TODO: use secKey & pubKey
    neighbourKeyMap.clear();
    crypto.SetKey(secKey, pubKey);
    QSettings p2pSetting("p2p.cfg",QSettings::IniFormat);
    QIPEndPoint local(p2pSetting.value("Local").toString());
    QIPEndPoint natServer(p2pSetting.value("NATServer").toString());
    QIPEndPoint p2pServer(p2pSetting.value("P2PServer").toString());
    server.Init(local, p2pServer);
    p2p.Init(crypto.getAddr(),natServer,local);
    SetPort(StartPort);
    timeOut.setSingleShot(true);

#ifdef TEST
    timeSync.StartTestSync(150);//Timer Simulation Test
    //timeSync.StartTestSync(1000);//Timer Simulation Test
#endif
}

void NClientInterface::SetPort(int port)
{
    ipc.SetIPCPort(port);
}

QString NClientInterface::getID()
{
    return crypto.getAddr();
}

QStringList NClientInterface::GetMemberList()
{
    return p2p.neighbourList();
}

void NClientInterface::SetLocalID(QString ID)
{
    SendLocalMsg("ID_",ID);
}

void NClientInterface::SendChatMsg(QString addr, QString msg)
{
    QJsonObject obj;
    obj["cmd"] = "message";
    obj["data"] = msg;
    //p2p.sendMsg();
    //p2p.sendByID(addr,QString(QJsonDocument(obj).toJson(QJsonDocument::Compact)));
}

void NClientInterface::StartTest()
{
    timeSync.StartTestSync(3000);
}

void NClientInterface::EnterLobby()
{
    server.Query(SV_CMD_ENTER+p2p.getLocalInfoString() + ";" + crypto.getPubKeyStr());
}

void NClientInterface::StartSoloQueue()
{
    server.Query(SV_CMD_SOLO+getID());
}

void NClientInterface::JoinTank()
{
    //onn.JoinGame(crypto.getSecKey().toHex().toUpper(),crypto.getPubKey().toHex().toUpper());
}

void NClientInterface::OnInit(QString msg)
{
    QString InitMsg;
    SendLocalMsg("REQ",InitMsg);
}

void NClientInterface::OnTick(int frameNo)
{
    //1.向客户端请求本地控制命令；
    //qDebug()<<__FUNCTION__<<frameNo;
    packer.frame = frameNo;
    QJsonObject obj;
    obj.insert("frame", frameNo);
    SendLocalMsg("REQ",QString(QJsonDocument(obj).toJson()));
}

#define CMDSIZE 3
void NClientInterface::OnRcvLocal(QString msg, QHostAddress senderIP, quint16 senderPort)
{
    //qDebug()<<__FUNCTION__<<msg;
    if(msg.size()<CMDSIZE){
        return;
    }
    auto cmd = msg.left(CMDSIZE);
    auto data = msg.mid(CMDSIZE);

    if(cmd == "STT"){
        RcvStart(data);
    }
    if(cmd == "CAU"){
        RcvLocalCause(data);
    }
    if(cmd == "RES"){
        RcvLocalResult(data);
    }
    if(cmd == "FIN"){
        //TODO:report result to main network
        //解析业务结果
        //OnFinish(data);
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
    if(cmd == "message"){
        emit RcvMsg(addr,obj["data"].toString());
    }
}

void NClientInterface::OnLobbyNet(QString dat)
{
    p2p.SetP2PList(dat);
}

void NClientInterface::OnGameNet(QString dat)
{
    p2p.SetP2PList(dat);
}

void NClientInterface::OnGameStart(QString dat)
{
    qDebug()<<__FUNCTION__<<__LINE__<<dat;
    SendGameInitInfo(dat);
}

void NClientInterface::OnGameTick(QString dat)
{
    OnTick(0);
}

void NClientInterface::OnFinish(QString msg)
{
    //TODO:推送结果给远程服务
    Game_Over(msg);
}

QString JsonPackCmd(QString data){
    QJsonObject obj = QJsonDocument::fromJson(data.toLatin1()).object();
    QJsonObject root;
    QJsonArray arr;
    arr.append(obj);
    root.insert("cmds",arr);
    return QString(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

void NClientInterface::RcvLocalCause(QString data)
{
    //2.获取到当前帧收集到的本地控制命令，广播

    //Send to ONN Server
#ifdef TEST
    //onn.SendMsg(data);
    QString packString = JsonPackCmd(data);
    SendLocalMsg("CAU",packString);//TestCode

    //SendLocalMsg("CAU",data);
#endif

    QJsonObject obj = QJsonDocument::fromJson(data.toLatin1()).object();
    quint64 frame = obj["frame"].toDouble();
    QString dataString = obj["data"].toString();
    packer.Push(getID(),dataString);
    timeOut.start(500);

    QJsonObject sobj;
    sobj.insert("frame",(int)frame);
    sobj.insert("addr",getID());
    sobj.insert("cmd","cause");
    sobj.insert("data",dataString);
    CryptoBroadcast(QString(QJsonDocument(sobj).toJson()));//广播本地控制命令
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

void NClientInterface::OnRcvP2P(QString msg)
{
    auto res = crypto.decode(msg,neighbourKeyMap);
    if(res.size()>=3){
        OnRcvNet(res[0].toInt(),res[1],res[2]);
    }
}

void NClientInterface::OnRcvServerMsg(QString cmd, QString msg)
{
    if(cmd == SV_CMD_LOBBY_NET) OnLobbyNet(msg);
    if(cmd == SV_CMD_GAME_NET) OnGameNet(msg);
    if(cmd == SV_CMD_GAME_START) OnGameStart(msg);
    if(cmd == SV_CMD_TICK) OnGameTick(msg);
}

void NClientInterface::OnReadyJoin()
{
    EnterLobby();
}

void NClientInterface::OnOnnMsg(QString msg)
{
    qDebug()<<__FUNCTION__<<__LINE__<<msg;
    auto res = QJsonDocument::fromJson(msg.toLatin1()).array();
    if(res.count()<=0){
        qDebug()<<__FUNCTION__<<__LINE__;
        return;
    }

    if(!res[0].isObject()){
        qDebug()<<__FUNCTION__<<__LINE__;
        return;
    }

    QJsonObject obj = res[0].toObject();

    if(!obj.contains("method")){
        qDebug()<<__FUNCTION__<<__LINE__;
        return;
    }

    auto method = obj["method"].toString();

    if(method == "startGame"){
        QString jsonMsg = QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
        SendGameInitInfo(jsonMsg);
    }

    if(method == "timeout"){
        OnTick(0);
        //ipc.Send("CAU" + msg);
    }
}

void NClientInterface::SendLocalMsg(QString cmd, QString msg)
{
    if(cmd.size()!=3){
        qDebug()<<"error cmd!!!"<< cmd;
        return;
    }
    ipc.Send(cmd+msg);
}

void NClientInterface::Enter_Lobby()
{
    server.Query(SV_CMD_ENTER + p2p.getLocalInfoString());
}

void NClientInterface::Queue_Solo()
{
    server.Query(SV_CMD_QUEUE_SOLO + getID());
}

void NClientInterface::SendGameInitInfo(QString data)
{
    SendLocalMsg("INI", data);
}

void NClientInterface::BroadcastCause()
{
    //4.广播接收到的打包操作命令
    timeOut.stop();
    auto jsonDat = packer.PackJsonString();
    auto frame = packer.frame;
    consensus.RcvCause(frame,getID(),jsonDat);//本地操作命令参与共识
    QJsonObject obj;
    obj.insert("frame",frame);
    obj.insert("addr",getID());
    obj.insert("cmd","causePark");
    obj.insert("data",jsonDat);
    CryptoBroadcast(QString(QJsonDocument(obj).toJson()));
}

void NClientInterface::RcvStart(QString data)
{
    SetLocalID(crypto.getAddr());
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
    consensus.RcvResult(frame, getID(),dataString);

    QJsonObject sobj;
    sobj.insert("frame",(int)frame);
    sobj.insert("addr",getID());
    sobj.insert("cmd","result");
    sobj.insert("data",dataString);
    CryptoBroadcast(QString(QJsonDocument(sobj).toJson()));//广播本地结果
}

void NClientInterface::RcvNetResult(quint64 frame, QString addr, QString data)
{
    //7.接收网络执行结果，共识，并推入区块,甄别作弊者
    consensus.RcvResult(frame,addr,data);
}

void NClientInterface::CryptoSend(QString id, QString msg)
{
    auto cmsg = crypto.encode(msg);
    p2p.sendMsg(id,cmsg);
}

void NClientInterface::CryptoBroadcast(QString msg)
{
    auto cmsg = crypto.encode(msg);
    p2p.broadcastMsg(cmsg);
}
