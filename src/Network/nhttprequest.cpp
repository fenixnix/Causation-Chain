#include "nhttprequest.h"

NHttpRequest::NHttpRequest(QObject *parent) : QObject(parent)
{
    connect(&qnam, &QNetworkAccessManager::finished, this, &NHttpRequest::OnFinished);
}

void NHttpRequest::SelfTest()
{

}

void NHttpRequest::Get(QString url)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<url;
    //QNetworkReply *reply =
    qnam.get(QNetworkRequest(url));
    //connect(reply, &QNetworkReply::finished, this, &NHttpRequest::OnFinish);
    //connect(reply, &QIODevice::readyRead, this, &NHttpRequest::OnReadReady);
}

void NHttpRequest::Post(QString url, QByteArray data)
{
    auto req = QNetworkRequest(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply *reply = qnam.post(req,data);
    //connect(reply, &QNetworkReply::finished, this, &NHttpRequest::OnFinish);
    //connect(reply, &QIODevice::readyRead, this, &NHttpRequest::OnReadReady);
}

void NHttpRequest::OnReadReady()
{
    auto re = (QNetworkReply*)this->sender();
    QByteArray data = re->readAll();
    //qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<QString(data);
    emit RcvMsg(QString(data));
}

#include <iostream>
using namespace std;
void NHttpRequest::OnFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    //qDebug() << QTime::currentTime().toString("ss zzz") << " : "<<data;
    emit RcvMsg(QString(data));
    reply->deleteLater();
}

void NHttpRequest::OnFinish()
{
    //qDebug()<<__FILE__<<__FUNCTION__<<__LINE__;

    auto re = (QNetworkReply*)this->sender();
    re->deleteLater();
}
