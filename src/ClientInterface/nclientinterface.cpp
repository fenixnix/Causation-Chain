#include "nclientinterface.h"
#include "wtccmddefine.h"
#include "nhttprequest.h"
#include "ndatastore.h"

QString sendBuffer;

NDataStore ds;

#define TICKINTERVAL 150

NClientInterface::NClientInterface(QObject *parent) : QObject(parent)
{
    NSimpleStore::SelfTest();
    connect(&ipc, &UdpNetwork::Rcv, this, &NClientInterface::OnRcvLocal);
    connect(&loadTimer, &QTimer::timeout, this, &NClientInterface::OnLoad);

#ifdef ONN
    connect(&onn, &OnnConnector::StartGame, this, &NClientInterface::OnStartGame);
    connect(&onn, &OnnConnector::Tick, this, &NClientInterface::OnOnnTick);
    connect(&onn, &OnnConnector::LoopTick, this, &NClientInterface::OnLoopTick);
    connect(this, &NClientInterface::TrigOnnTick, &onn, &OnnConnector::GetTick,Qt::QueuedConnection);
    connect(this, &NClientInterface::OnnInitSign, &onn, &OnnConnector::Init,Qt::QueuedConnection);
    connect(this, &NClientInterface::OnnJoinSign, &onn, &OnnConnector::JoinGame,Qt::QueuedConnection);
    connect(this, &NClientInterface::OnnCloseSign, &onn, &OnnConnector::CloseGame,Qt::QueuedConnection);
    connect(this, &NClientInterface::OnnPlaySign, &onn, &OnnConnector::PlayGame,Qt::QueuedConnection);
#endif
    connect(&timeSync, &NTimeSync::Tick, this, &NClientInterface::OnTick);

    Init();
}

NClientInterface::~NClientInterface()
{
#ifdef ONN
    onn.terminate();
#endif
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
#ifdef ONN
    onn.moveToThread(&onn);
    onn.start();
    emit OnnInitSign(crypto.getSecKey().toHex().toUpper(),crypto.getPubKey().toHex().toUpper());
#endif
    causeStore.Init("cause.dat");
    resultStore.Init("result.dat");
}

void NClientInterface::Init(QString secKey, QString pubKey)
{
    crypto.SetKey(secKey, pubKey);
    //QSettings p2pSetting("p2p.cfg",QSettings::IniFormat);
    //QIPEndPoint local(p2pSetting.value("Local").toString());
    //QIPEndPoint natServer(p2pSetting.value("NATServer").toString());
    //p2p.Init(crypto.getAddr(),natServer,local);
    ipc.SetIPCPort(StartPort);
}

QString frameString(int frm){
    QJsonObject obj;
    obj.insert("frm", frm);
    return JSON2STRING(obj);
}

QString NClientInterface::GetID()
{
#ifdef ONN
    return crypto.getEthAddr();
#endif
    return "P1";
}

QString NClientInterface::GetUrl(){
#ifdef ONN
    return onn.GetUrl();
#else
    return "";
#endif
}

QString NClientInterface::GetContract(){
#ifdef ONN
    return onn.GetContract();
#else
    return "";
#endif
}

void NClientInterface::JoinTank()
{
    emit OnnJoinSign();
}

void NClientInterface::CloseTank()
{
    emit OnnCloseSign();
}

void NClientInterface::LoadTank()
{
    causeStore.Load("cause.dat");
#ifndef ONN
    loadTimer.start(TICKINTERVAL);
#endif
}

void NClientInterface::OnTick(int frm)
{
    //    bool isEven = !(frameNo&0x1);
    //    int frm = frameNo>>1;
    //    if(isEven){
    //        //1.向客户端请求本地CMD,和上一帧输入共识
    //        //packer.frame = frm;
    //        //TODO: 输入共识：if(consensus)
    //        //ipc.Send("CAU",consensus.ReachCauseConsensus(););
    //        SendLocalMsg("REQ",frameString(frm));
    //    }else{
    //        //BroadcastCause();
    //    }
#ifndef ONN
    auto obj = STRING2JSON(sendBuffer);
    obj["frm"] = frm;
    auto JsonPackCmdString = JSON2STRING(obj);
    SendLocalMsg("CAU",JsonPackCmdString);
    causeStore.Push(JsonPackCmdString);
#endif
}

#define CMDSIZE 3
void NClientInterface::OnRcvLocal(QString msg, QHostAddress senderIP, quint16 senderPort)
{
    CHECK_RETURN(msg.size()<CMDSIZE);
    auto cmd = msg.left(CMDSIZE);
    auto data = msg.mid(CMDSIZE);
    if(cmd == "CAU")RcvLocalCause(data);
    if(cmd == "RES")RcvLocalResult(data);
    //if(cmd == "FIN"){//TODO:report result to main network}
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
    //qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<data;
#ifdef ONN
    emit OnnPlaySign(data);
#endif

#ifndef ONN
    auto obj = STRING2JSON(data);
    QJsonArray array;
    array.append(obj);
    QJsonObject pack;
    //pack["frm"] = 0;
    pack["msg"] = array;
    auto JsonPackCmdString = JSON2STRING(pack);
    sendBuffer = JsonPackCmdString;
#endif

    //    auto obj = QJsonDocument::fromJson(data).object();
    //    QJsonArray array;
    //    array.append(obj);
    //    QJsonObject jobj["msg"] = array;
    //    auto jsonMsg = QString(QJsonDocument(jobj).toJson());
    //    SendLocalMsg("CAU", jsonMsg);
    //    SendLocalMsg("REQ","";
}

void NClientInterface::OnOnnTick(int frame, QString msg)
{
    //qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<msg;
    QJsonObject timeOutObj;
    QJsonArray srcArray = QJsonDocument::fromJson(msg.toLatin1()).array();
    QJsonArray dstArray;
    for(int i = 0;i<srcArray.size();i++){
        dstArray.append(QJsonDocument::fromJson(srcArray[i].toString().toLatin1()).object());
    }
    timeOutObj["frm"] = frame;
    timeOutObj["msg"] = dstArray;
    QString jsonMsg = QString(QJsonDocument(timeOutObj).toJson(QJsonDocument::Compact));
    //qDebug()<<__FUNCTION__<<__LINE__<<frame;

    SendLocalMsg("CAU", jsonMsg);
    causeStore.Push(jsonMsg);
    //SendLocalMsg("REQ",frameString(frame));
    //qDebug()<<__FUNCTION__<<__LINE__;
}

void NClientInterface::OnStartGame(QString jsonArrayMembers)
{
    //causeStore.Push(jsonArrayMembers);
    auto memberArray = QJsonDocument::fromJson(jsonArrayMembers.toLatin1()).array();
    QJsonObject obj;
    obj["locID"] = GetID();
    obj["members"] = memberArray;
    auto initString = JSON2STRING(obj);
    qDebug()<<__FUNCTION__<<__LINE__<<initString;
    SendLocalMsg("INI", initString);

#ifdef ONN
    //for ONN
    emit TrigOnnTick(1);//start tick
#endif
}

void NClientInterface::OnLoopTick(int frm)
{
    //qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<frm;
    emit TrigOnnTick(frm);
}

void NClientInterface::SendLocalMsg(QString cmd, QString msg)
{
    if(cmd.size()!=3){
        qDebug()<<"error cmd!!!"<< cmd;
        return;
    }
    ipc.Send(cmd+msg);
}

void NClientInterface::StartTestTick()
{
#ifndef ONN
    timeSync.StartTestSync(TICKINTERVAL);
#endif
}

void NClientInterface::RcvLocalResult(QString data)
{
    //6.接收本地执行结果，并广播
    //qDebug()<<__FUNCTION__<<data;
    resultStore.Push(data);
    //    QJsonObject obj = QJsonDocument::fromJson(data.toLatin1()).object();
    //    quint64 frame = obj["frame"].toDouble();
    //    QString dataString = obj["data"].toString();
    //    consensus.RcvResult(frame, getID(),dataString);

    //    QJsonObject sobj;
    //    sobj.insert("frame",(int)frame);
    //    sobj.insert("addr",getID());
    //    sobj.insert("cmd","result");
    //    sobj.insert("data",dataString);
    //    CryptoBroadcast(QString(QJsonDocument(sobj).toJson()));//广播本地结果
}

void NClientInterface::OnLoad()
{
    SendLocalMsg("CAU",causeStore.Read());
}

//void NClientInterface::RcvNetResult(quint64 frame, QString addr, QString data)
//{
//    //7.接收网络执行结果，共识，并推入区块,甄别作弊者
//    consensus.RcvResult(frame,addr,data);
//}
