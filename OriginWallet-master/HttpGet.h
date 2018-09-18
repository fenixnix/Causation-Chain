#ifndef HTTPGET_H
#define HTTPGET_H

#include "Define.h"
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>

class HttpGet : public QObject
{
    Q_OBJECT
public:
    explicit HttpGet(){
        networkAccessManager = new QNetworkAccessManager(this);
        QObject::connect(networkAccessManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(finishedSlot(QNetworkReply *)),Qt::QueuedConnection);
    }

public slots:
    void onGet(QUrl url){
        const QNetworkRequest request = QNetworkRequest(url);
        //这里可以加入chrome的请求头部，使其更接近浏览器
        //request.setHeader(QNetworkRequest::UserAgentHeader, QVariant("Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.76 Mobile Safari/537.36"));
        networkAccessManager->get(request);
    }
    void finishedSlot(QNetworkReply *reply){
        if(reply->error() == QNetworkReply::NoError) {
            QByteArray buffer = reply->readAll();
            //QString str = QString::fromUtf8(buffer);
            //qDebug()<<str;
            BUG << buffer;
            emit doGetData(buffer);
        } else {
            BUG<<reply->errorString();
        }
        reply->deleteLater();
    }
signals:
    void doGetData(QString);
private:
    QNetworkAccessManager *networkAccessManager;
};

#endif // HTTPGET_H
