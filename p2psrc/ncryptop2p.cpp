#include "ncryptop2p.h"
#include <QJsonDocument>

NCryptoP2P::NCryptoP2P(QObject *parent) : QObject(parent)
{
    QObject::connect(&p2p,&NP2PNode::RcvMsg,this,&NCryptoP2P::rcvP2PMsg);
}

void NCryptoP2P::GenerateKey()
{
    NEmcc ecc;
    ecc.GenerateKeyPair();
    QSettings cryptoSetting("crypto.cfg", QSettings::IniFormat);
    cryptoSetting.setValue("SecKey", ecc.privateKeyString);
    cryptoSetting.setValue("PubKey", ecc.publicKeyString);
    cryptoSetting.sync();
}

void NCryptoP2P::Init()
{
    QSettings cryptoSetting("crypto.cfg", QSettings::IniFormat);
    QString secKey = cryptoSetting.value("SecKey").toString();
    QString pubKey = cryptoSetting.value("PubKey").toString();
    emcc.SetSecKey(secKey);
    emcc.SetPubKey(pubKey);
    QString addr = emcc.address;

    QSettings p2pSetting("p2p.cfg",QSettings::IniFormat);
    QIPEndPoint local(p2pSetting.value("Local").toString());
    QIPEndPoint natServer(p2pSetting.value("NATServer").toString());
    QIPEndPoint p2pServer(p2pSetting.value("P2PServer").toString());
    p2p.Init(addr,natServer,local);
}

QStringList NCryptoP2P::getP2pMemberList()
{
    return p2p.neighbourList();
}

QString NCryptoP2P::sendByID(QString id, QString msg)
{
    //qDebug()<<__FUNCTION__<<id<<msg;
    auto dMsg = Dsa(msg);
    p2p.sendMsg(dMsg,id);
    return dMsg;
}

QString NCryptoP2P::localAddr()
{
    return emcc.address;
}

void NCryptoP2P::boardcastMsg(QString msg)
{
    p2p.broadcastMsg(Dsa(msg));
}

void NCryptoP2P::SelfTest()
{
    NCryptoP2P cp2p;
    cp2p.Init();
    QString msg = "hello world!!!";
    auto cMsg = cp2p.sendByID("Nix",msg);
    qDebug()<<"MSG:"<<msg;
    qDebug()<<"CMSG:"<<cMsg;
    cp2p.rcvP2PMsg(cMsg);
}

void NCryptoP2P::rcvP2PMsg(QString msg)
{
    //qDebug()<<__FUNCTION__<<msg;
    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    QString dat = obj["DAT"].toString();
    QString sign = obj["SIGN"].toString();
    QJsonObject jsonDat = QJsonDocument::fromJson(dat.toLatin1()).object();
    //qDebug()<<"RcvData:"<<dat;
    QString pubKey = jsonDat["PK"].toString();
    if(!NEmcc::VerifyMsg(pubKey,dat,sign)){
        qDebug()<<"Faked data!!!"<<msg;
        return;
    }
    auto addr = NEmcc::CalcAddr(pubKey);
    quint64 timeStamp = jsonDat["TS"].toDouble();
    QString rawMsg = jsonDat["MSG"].toString();
    qDebug()<<"RcvVerifyMsg:"<<timeStamp<<"+"<<addr<<"+"<<rawMsg;
    emit RcvMsg(timeStamp,addr,rawMsg);
}

QString NCryptoP2P::Dsa(QString msg)
{
    QJsonObject jsonObj;
    jsonObj.insert("TS",QDateTime::currentMSecsSinceEpoch());
    jsonObj.insert("MSG",msg);
    jsonObj.insert("PK",emcc.publicKeyString);
    QJsonDocument jDom(jsonObj);
    QString jsonDatString = QString(jDom.toJson(QJsonDocument::Compact));
    auto sign = emcc.Sign(jsonDatString);
    QJsonObject sendJsonObj;
    sendJsonObj.insert("DAT",jsonDatString);
    sendJsonObj.insert("SIGN",sign);
    QJsonDocument jSendDom(sendJsonObj);
    QString sendJsonDatString = QString(jSendDom.toJson(QJsonDocument::Compact));
    return sendJsonDatString;
}
