#include "nhttprequest.h"

NHttpRequest::NHttpRequest(QObject *parent) : QObject(parent)
{

}

void NHttpRequest::SelfTest()
{

}

void NHttpRequest::Get(QString url)
{
    //qDebug()<<__FUNCTION__<<__LINE__;
    QNetworkReply *reply = qnam.get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, &NHttpRequest::OnFinish);
    connect(reply, &QIODevice::readyRead, this, &NHttpRequest::OnReadReady);
}

void NHttpRequest::Post(QString url, QByteArray data)
{
    QNetworkReply *reply = qnam.post(QNetworkRequest(QUrl(url)),data);
    connect(reply, &QNetworkReply::finished, this, &NHttpRequest::OnFinish);
    connect(reply, &QIODevice::readyRead, this, &NHttpRequest::OnReadReady);
}

void NHttpRequest::OnReadReady()
{
    auto re = (QNetworkReply*)this->sender();
    QByteArray data = re->readAll();
    //qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<QString(data);
    emit RcvMsg(QString(data));
}

void NHttpRequest::OnFinish()
{
    //qDebug()<<__FILE__<<__FUNCTION__<<__LINE__;
    auto re = (QNetworkReply*)this->sender();
    re->deleteLater();
}
