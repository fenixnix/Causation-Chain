#include "netsync.h"
#include <QJsonDocument>

NetSync::NetSync(QObject *parent) : QObject(parent)
{
    Init();
}

void NetSync::Init()
{
    ecDsa.GenerateKeyPair();

    QSettings setting("config",QSettings::IniFormat);
    auto id = ecDsa.address;
    p2p.setID(id);
    qDebug()<<"ID:"<<id;

    QIPEndPoint local(setting.value("Local").toString());
    QIPEndPoint nat(setting.value("NATServer").toString());
    QIPEndPoint p2pA(setting.value("P2PServer").toString());

    p2p.bindLocalEndPoint(QIPEndPoint(local.IP(),local.Port()));
    p2p.setP2PServer(p2pA);
    p2p.join(nat);

    QObject::connect(&p2p,&NP2PNode::RcvMsg,this,&NetSync::RcvP2pMsg);
}

QString NetSync::BoardcastBlockChainLevel(QString id, QString level)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.address);
    obj.insert("Level",level);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.boardcastMsg(signedMsg);
    return signedMsg;
}

QString NetSync::RequireBlockChainData(QString id, QString nodeAddress, QString start, QString end)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.address);
    obj.insert("Start",start);
    obj.insert("End",end);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.sendbyID(signedMsg,nodeAddress);
    return signedMsg;
}

QString NetSync::SendBlockChainData(QString id, QString nodeAddress, QString data)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.address);
    obj.insert("Data",data);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.sendbyID(signedMsg,nodeAddress);
    return signedMsg;
}

void NetSync::SelfTest()
{
    auto msg0 = BoardcastBlockChainLevel("ONN","100");
    auto msg1 = RequireBlockChainData("ONN","Node Addresss1","50","60");
    auto msg2 = SendBlockChainData("ONN","NodeAddress2","BlockChainData");
    RcvP2pMsg(msg0);
    RcvP2pMsg(msg1);
    RcvP2pMsg(msg2);
}

void NetSync::RcvP2pMsg(QString signedMsg)
{
    QJsonDocument jDom = QJsonDocument::fromJson(signedMsg.toLatin1());
    qDebug()<<__FUNCTION__<<QString(jDom.toJson());
    auto msg = jDom["Msg"].toString();
    auto pubKey = jDom["PubKey"].toString();
    auto sign = jDom["Sign"].toString();
    if(!ecDsa.VerifyMsg(pubKey,msg,sign)){
        return;
    }

    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    QString addr = obj["Addr"].toString();
    QString contractID = obj["ID"].toString();
    if(obj.contains("Level")){
        emit RcvBlockChainLevel(contractID,addr,obj["Level"].toString());
        qDebug()<<"Rcv:Level"<<contractID<<addr<<obj["Level"].toString();
    }
    if(obj.contains("Start")){
        emit RcvBlockChainDataRequire(contractID,addr,obj["Start"].toString(),obj["End"].toString());
        qDebug()<<"Rcv:Require"<<contractID<<addr<<obj["Start"].toString()<<obj["End"].toString();
    }
    if(obj.contains("Data")){
        emit RcvBlockChainData(contractID,addr,obj["Data"].toString());
        qDebug()<<"Rcv:Data"<<contractID<<addr<<obj["Data"].toString();
    }
}

QString NetSync::setUpSignedMsg(QString msg)
{
    qDebug()<<__FUNCTION__<<msg;
    QString msgHashSign = ecDsa.Sign(msg);
    QJsonObject obj;
    obj.insert("Msg",msg);
    obj.insert("Sign",msgHashSign);
    obj.insert("PubKey",ecDsa.publicKeyString);
    QJsonDocument jdom(obj);
    QString jsonString = QString(jdom.toJson());
    qDebug()<<jsonString;
    return jsonString;
}
