#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include <iostream>
#include <QList>
#include <QString>
#include <stdio.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QThreadPool>
#include <QTime>
#include <QCryptographicHash>
#include <QFile>
#include <QNetworkRequest>
#include <QtNetwork>
#include "emcc/uECC.h"

#define BUG qDebug()<<__FUNCTION__<<__LINE__

#define GETMD5(A) QCryptographicHash::hash(A,QCryptographicHash::Md5).toHex()
#define GETADDR(A) QCryptographicHash::hash(A,QCryptographicHash::Keccak_256).toHex().left(40)
#define GETSHA256(A) QCryptographicHash::hash(A,QCryptographicHash::Sha256).toHex()

using namespace std;

class HttpRequest {
public:
    HttpRequest(){}
    ~HttpRequest(){}

    static QByteArray qtGet(const QString &strUrl)
    {
        BUG << strUrl;

        const QUrl url = QUrl::fromUserInput(strUrl);
        QNetworkAccessManager m_qnam;
        QByteArray replyData;

        QNetworkRequest qnr(url);
        QNetworkReply* reply = m_qnam.get(qnr); //m_qnam是QNetworkAccessManager对象

        QEventLoop eventLoop;
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        //eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
        eventLoop.exec();

        replyData = reply->readAll();

        reply->deleteLater();
        reply = nullptr;

        return replyData;
    }

    static QByteArray qtPost(const QString &strUrl,QByteArray pData)
    {
        BUG;

        const QUrl url = QUrl::fromUserInput(strUrl);
        QNetworkAccessManager m_qnam;
        QByteArray replyData;

        QNetworkRequest qnr(url);
        qnr.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        QNetworkReply* reply = m_qnam.post(qnr,pData); //m_qnam是QNetworkAccessManager对象

        QEventLoop eventLoop;
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        //eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
        eventLoop.exec();

        replyData = reply->readAll();

        reply->deleteLater();
        reply = nullptr;

        return replyData;
    }

    static QString getTaget(QString type,QString pubkey,QString name,QString code,QString arg,QString sig) {
        QString data = type + "$" + pubkey + "$" + name + "$" + code + "$" + arg;
        QString block = sig + "&" + data;
        return block;
    }

    static QString docmd(QString type,QString pubkey,QString prikey,QString name,QString func,QString arg) {
        BUG << type << pubkey;
        QString msg = type + "$" + pubkey + "$" + name + "$" + func + "$" + arg;
        QByteArray hash = GETSHA256(msg.toLatin1());
        sign(prikey.toLatin1().data(),hash.data());
        QByteArray sig = getSign();
        sig = sig.left(128);
        QString block = getTaget(type, pubkey, name, func, arg, sig);
        return block;
    }

//#define IP "http://47.75.190.195:3000"
#define IP "http://192.168.1.156:3000"

    static QJsonArray doMethodGet(QByteArray pubKey, QString pMethod = "", QString pArg = "null", QString pContract = "TANK2", QString url = IP){
        QString block = pContract+"$"+pMethod+"$"+pArg+"$"+pubKey;
        BUG << block;
        QByteArray result = qtGet(url+"/"+block);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(result);
        QJsonArray    jsonArr = jsonDoc.array();
        BUG << jsonArr;
        return jsonArr;
    }

    static QByteArray doMethodSet(QByteArray secKey, QByteArray pubKey,QString pMethod = "joinGame",QString pArg = "null" ,QString pContract = "TANK2", QString url = IP){
        BUG << pubKey;
        QString block = docmd("method",pubKey,secKey,pContract,pMethod,pArg);
        BUG << url << block;
        //return qtGet(url+"/"+block);
        return qtPost(url,block.toLatin1());
    }
};

#endif // HTTPREQUEST_H
