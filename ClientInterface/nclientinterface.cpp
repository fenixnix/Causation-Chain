#include "nclientinterface.h"
#include "wtccmddefine.h"

NClientInterface::NClientInterface(QObject *parent) : QObject(parent)
{
    connect(&timeSync, &NTimeSync::Tick, this, &NClientInterface::OnTick);
    connect(&ipc, &UdpNetwork::Rcv, this, &NClientInterface::OnRcvLocal);
    connect(&p2p, &NP2PNode::RequireJoin, this, &NClientInterface::OnReadyJoin);
    connect(&p2p, &NP2PNode::RcvMsg, this, &NClientInterface::OnRcvP2P);
    connect(&tcpServer, &NTcpNetwork::ClientRcvMsg, this, &NClientInterface::OnRcvServerMsg);

    connect(&onn, &OnnConnector::StartGame, this, &NClientInterface::OnStartGame);
    connect(&onn, &OnnConnector::Tick, this, &NClientInterface::OnOnnTick);
    connect(&onnTimer, &QTimer::timeout, this, &NClientInterface::OnOnnTimer);
    connect(this, &NClientInterface::TrigOnnTick, &onn, &OnnConnector::GetTick,Qt::QueuedConnection);
    connect(this, &NClientInterface::OnnInitSign, &onn, &OnnConnector::Init,Qt::QueuedConnection);
    connect(this, &NClientInterface::OnnJoinSign, &onn, &OnnConnector::JoinGame,Qt::QueuedConnection);
    connect(this, &NClientInterface::OnnPlaySign, &onn, &OnnConnector::PlayGame,Qt::QueuedConnection);

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

    onn.moveToThread(&onn);
    onn.start();
    qDebug()<<__FUNCTION__<<this->thread();
    emit OnnInitSign();
    //onn.Init();
}

void NClientInterface::Init(QString secKey, QString pubKey)
{
    neighbourKeyMap.clear();
    crypto.SetKey(secKey, pubKey);
    QSettings p2pSetting("p2p.cfg",QSettings::IniFormat);
    QIPEndPoint local(p2pSetting.value("Local").toString());
    QIPEndPoint natServer(p2pSetting.value("NATServer").toString());
    //p2p.Init(crypto.getAddr(),natServer,local);
    ipc.SetIPCPort(StartPort);
}

QString frameString(int frm){
    QJsonObject obj;
    obj.insert("frm", frm);
    return JSON2STRING(obj);
}

QString NClientInterface::getID()
{
    return crypto.getAddr();
}

QStringList NClientInterface::GetMemberList()
{
    return p2p.neighbourList();
}

void NClientInterface::SendChatMsg(QString addr, QString msg)
{
    QJsonObject obj;
    obj["cmd"] = "msg";
    obj["dat"] = msg;
    //p2p.sendMsg();
    //p2p.sendByID(addr,QString(QJsonDocument(obj).toJson(QJsonDocument::Compact)));
}

void NClientInterface::Enter_Lobby()
{
    QString msg = SV_CMD_ENTER + p2p.getLocalInfoString();
    tcpServer.Send2Server(msg);
}

void NClientInterface::JoinTank()
{
    //onn.JoinGame(crypto.getSecKey().toHex().toUpper(),crypto.getPubKey().toHex().toUpper());
    emit OnnJoinSign(crypto.getSecKey().toHex().toUpper(),crypto.getPubKey().toHex().toUpper());
}

void NClientInterface::OnnInputs(int frame, QString msg)
{
    QJsonObject timeOutObj;
    QJsonArray srcArray = QJsonDocument::fromJson(msg.toLatin1()).array();
    QJsonArray dstArray;
    for(int i = 0;i<srcArray.size();i++){
        dstArray.append(QJsonDocument::fromJson(srcArray[i].toString().toLatin1()).object());
    }
    timeOutObj["msg"] = dstArray;
    QString jsonMsg = QString(QJsonDocument(timeOutObj).toJson(QJsonDocument::Compact));
    //qDebug()<<__FUNCTION__<<__LINE__<<jsonMsg;
    SendLocalMsg("CAU", jsonMsg);
    SendLocalMsg("REQ",frameString(frame));
    //qDebug()<<__FUNCTION__<<__LINE__;
}

void NClientInterface::OnTick(int frameNo)
{
    bool isEven = !(frameNo&0x1);
    int frm = frameNo>>1;
    if(isEven){
        //1.向客户端请求本地CMD,和上一帧输入共识
        packer.frame = frm;
        //TODO: 输入共识：if(consensus)
        //ipc.Send("CAU",consensus.ReachCauseConsensus(););
        SendLocalMsg("REQ",frameString(frm));
    }else{
        BroadcastCause();
    }
}

#define CMDSIZE 3
void NClientInterface::OnRcvLocal(QString msg, QHostAddress senderIP, quint16 senderPort)
{
    //qDebug()<<__FUNCTION__<<msg;
    CHECK_RETURN(msg.size()<CMDSIZE);
    auto cmd = msg.left(CMDSIZE);
    auto data = msg.mid(CMDSIZE);

    if(cmd == "CAU")RcvLocalCause(data);
    if(cmd == "RES")RcvLocalResult(data);
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

void NClientInterface::OnSubNet(QString dat)
{
    qDebug()<<__FUNCTION__<<__LINE__<<dat;
    p2p.SetP2PList(dat);
}

void NClientInterface::OnGameTick(QString dat)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<dat;
    auto obj = QJsonDocument::fromJson(dat.toLatin1()).object();
    CHECK_RETURN(!obj.contains("frm"));
    int frame = obj["frm"].toInt();
    OnTick(frame);
}

static QString JsonPackCmd(QString data){
    QJsonObject obj = QJsonDocument::fromJson(data.toLatin1()).object();
    QJsonObject root;
    QJsonArray arr;
    arr.append(obj);
    root.insert("msg",arr);
    return QString(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

void NClientInterface::RcvLocalCause(QString data)
{
    //2.broadcast local cause input
    //qDebug()<<__FUNCTION__<<__LINE__<<data;
    //Send to ONN Server
    //onn.PlayGame(data);

    emit OnnPlaySign(data);

    //    QJsonObject obj = QJsonDocument::fromJson(data.toLatin1()).object();
    //    quint64 frame = obj["frm"].toDouble();
    //    QString dataString = obj["data"].toString();
    packer.Push(getID(),data);
    //timeOut.start(500);

    QJsonObject sobj;
    sobj.insert("frm",packer.frame);
    sobj.insert("id",getID());
    sobj.insert("dat",data);
    p2p.broadcastMsg(JSON2STRING(sobj));
    //CryptoBroadcast(QString(QJsonDocument(sobj).toJson()));//广播本地控制命令
}

void NClientInterface::RcvNetCause(quint64 frm, QString addr, QString data)
{
    //3.1收集其他节点的控制命令，在收集全部参与者的控制命令
    qDebug()<<__FUNCTION__<<__LINE__<<frm;
    if(packer.frame == frm){
        packer.Push(addr,data);
    }
}

void NClientInterface::OnRcvP2P(QString msg)
{
    qDebug()<<__FUNCTION__<<__LINE__<<msg;
    auto res = crypto.decode(msg,neighbourKeyMap);
    if(res.size()>=3){
        OnRcvNet(res[0].toInt(),res[1],res[2]);
    }
}

void NClientInterface::OnRcvServerMsg(QString msg)
{
    MessageProtocol mp;
    auto cmd = mp.Decode(msg);
    if(cmd==""){
        return;
    }
    OnRcvServerCmdMsg(cmd, mp.getData());
}

void NClientInterface::OnRcvServerCmdMsg(QString cmd, QString msg)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<cmd<<msg;
    if(cmd == SV_CMD_LOBBY_NET) OnSubNet(msg);
    if(cmd == SV_CMD_GAME_NET) OnSubNet(msg);
    if(cmd == SV_CMD_GAME_START) OnStartGame(msg);
    if(cmd == SV_CMD_TICK) OnGameTick(msg);
}

bool joined  = false;
void NClientInterface::OnReadyJoin()
{
    if(!joined){
        Enter_Lobby();
        joined = true;
    }
}

void NClientInterface::OnOnnTimer()
{
    //onn.GetTick(onnFrame);
    emit TrigOnnTick(onnFrame);
}

void NClientInterface::OnOnnTick(int frame, QString msg)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<frame;
    OnnInputs(frame,msg);
    if(frame == onnFrame){
        onnFrame++;
    }
    //qDebug()<<__FUNCTION__<<__LINE__<<msg;
}

void NClientInterface::OnStartGame(QString jsonArrayMembers)
{
    auto memberArray = QJsonDocument::fromJson(jsonArrayMembers.toLatin1()).array();
    QJsonObject obj;
    obj["locID"] = crypto.getEthAddr();
    obj["members"] = memberArray;
    auto initString = JSON2STRING(obj);
    qDebug()<<__FUNCTION__<<__LINE__<<initString;
    SendGameInitInfo(initString);

    //for ONN
    onnFrame = 1;
    onnTimer.start(200);
}

void NClientInterface::SendLocalMsg(QString cmd, QString msg)
{
    if(cmd.size()!=3){
        qDebug()<<"error cmd!!!"<< cmd;
        return;
    }
    ipc.Send(cmd+msg);
}

void NClientInterface::Queue_Solo()
{
    QString msg = SV_CMD_QUEUE_SOLO + getID()+";"+crypto.getPubKeyStr();
    tcpServer.Send2Server(msg);
}

void NClientInterface::Game_Over(QString Result)
{
    //TODO:推送结果给远程服务
    //tcpServer.Send2Server(SV_CMD_GAMEOVER + Result);
}

void NClientInterface::SendGameInitInfo(QString data)
{
    SendLocalMsg("INI", data);
}

void NClientInterface::BroadcastCause()
{
    //4.广播接收到的打包输入命令
    qDebug()<<__FUNCTION__<<__LINE__;
    auto jsonDat = packer.PackJsonString();
    auto frame = packer.frame;
    consensus.RcvCause(frame,getID(),jsonDat);//本地操作命令参与共识
    QJsonObject obj;
    obj.insert("frm",frame);
    obj.insert("id",getID());
    obj.insert("cmd","causePark");
    obj.insert("dat",jsonDat);
    p2p.broadcastMsg(JSON2STRING(obj));
    //CryptoBroadcast(QString(QJsonDocument(obj).toJson()));
}

void NClientInterface::RcvNetCausePack(quint64 frame, QString addr, QString data)
{
    //5.接收消息并 共识输入达成50%以上时执行。
    qDebug()<<__FUNCTION__<<__LINE__;
    consensus.RcvCause(frame,addr,data);
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
