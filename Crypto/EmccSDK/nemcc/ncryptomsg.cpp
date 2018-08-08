#include "ncryptomsg.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
#include <QDebug>
#include <QDateTime>

NCryptoMsg::NCryptoMsg()
{

}

void NCryptoMsg::GenerateKey()
{
    ecc.GenerateKeyPair();
}

void NCryptoMsg::SetKey(QString secKey, QString pubKey)
{
    ecc.SetSecKey(secKey);
    ecc.SetPubKey(pubKey);
}

void NCryptoMsg::LoadConfigFile(QString fileName)
{
    QSettings cryptoSetting(fileName, QSettings::IniFormat);
    QString secKey = cryptoSetting.value("SecKey").toString();
    QString pubKey = cryptoSetting.value("PubKey").toString();
    SetKey(secKey,pubKey);
}

QString NCryptoMsg::getAddr()
{
    return ecc.address;
}

QString NCryptoMsg::getEthAddr()
{
    return ecc.ethAddr;
}

QString NCryptoMsg::getPubKeyStr()
{
    return ecc.publicKeyString;
}

QByteArray NCryptoMsg::getSecKey()
{
    return ecc.priKey;
}

QByteArray NCryptoMsg::getPubKey()
{
    return ecc.pubKey;
}

QString NCryptoMsg::encode(QString msg)
{
    return Dsa(msg);
}

QStringList NCryptoMsg::decode(QString msg, QMap<QString, QString> keyMap)
{
    QStringList ls;
    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    QString id = obj["ID"].toString();
    QString dat = obj["DAT"].toString();
    QString sign = obj["SIGN"].toString();
    QJsonObject jsonDat = QJsonDocument::fromJson(dat.toLatin1()).object();
    QString pubKey = keyMap[id];
    if(!NEmcc::VerifyMsg(pubKey,dat,sign)){
        qDebug()<<"Faked data!!!"<<msg;
        return ls;
    }
    auto addr = NEmcc::CalcAddr(pubKey);
    QString timeStamp = jsonDat["TS"].toString();
    QString rawMsg = jsonDat["MSG"].toString();
    qDebug()<<"RcvVerifyMsg:"<<timeStamp<<"+"<<addr<<"+"<<rawMsg;
    ls.append(timeStamp);
    ls.append(id);
    ls.append(rawMsg);
    return ls;
}

QString NCryptoMsg::encodeWithPubKey(QString msg)
{
    return DsaWithPubKey(msg);
}

QStringList NCryptoMsg::decodeWithPubKey(QString msg)
{
    QStringList ls;
    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    QString id = obj["ID"].toString();
    QString dat = obj["DAT"].toString();
    QString sign = obj["SIGN"].toString();
    QString pubKey = obj["PK"].toString();
    QJsonObject jsonDat = QJsonDocument::fromJson(dat.toLatin1()).object();
    if(!NEmcc::VerifyMsg(pubKey,dat,sign)){
        qDebug()<<"Faked data!!!"<<msg;
        return ls;
    }
    QString timeStamp = jsonDat["TS"].toString();
    QString rawMsg = jsonDat["MSG"].toString();
    //qDebug()<<"RcvVerifyMsg:"<<timeStamp<<"+"<<id<<"+"<<rawMsg;
    ls.append(timeStamp);
    ls.append(id);
    ls.append(rawMsg);
    return ls;
}

QString NCryptoMsg::GetMsgTSString(QString msg)
{
    QJsonObject jsonObj;
    jsonObj.insert("TS",QDateTime::currentMSecsSinceEpoch());
    jsonObj.insert("MSG",msg);
    QJsonDocument jDom(jsonObj);
    QString jsonDatString = QString(jDom.toJson(QJsonDocument::Compact));

    return jsonDatString;
}

QString NCryptoMsg::Dsa(QString msg)
{
    QString jsonDatString = GetMsgTSString(msg);
    auto sign = ecc.Sign(jsonDatString);
    QJsonObject sendJsonObj;
    sendJsonObj.insert("ID",ecc.address);
    sendJsonObj.insert("DAT",jsonDatString);
    sendJsonObj.insert("SIGN",sign);
    QJsonDocument jSendDom(sendJsonObj);
    QString sendJsonDatString = QString(jSendDom.toJson(QJsonDocument::Compact));
    return sendJsonDatString;
}

QString NCryptoMsg::DsaWithPubKey(QString msg)
{
    QString jsonDatString = GetMsgTSString(msg);
    auto sign = ecc.Sign(jsonDatString);
    QJsonObject sendJsonObj;
    sendJsonObj.insert("ID",ecc.address);
    sendJsonObj.insert("DAT",jsonDatString);
    sendJsonObj.insert("SIGN",sign);
    sendJsonObj.insert("PK",ecc.publicKeyString);
    QJsonDocument jSendDom(sendJsonObj);
    QString sendJsonDatString = QString(jSendDom.toJson(QJsonDocument::Compact));
    return sendJsonDatString;
}
