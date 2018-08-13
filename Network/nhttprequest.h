#ifndef NHTTPREQUEST_H
#define NHTTPREQUEST_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NHttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit NHttpRequest(QObject *parent = nullptr);

    void SelfTest();

signals:
    void RcvMsg(QString msg);

public slots:
    void Get(QString url);
    void Post(QString url, QByteArray data);

private slots:
    void OnFinish();
    void OnReadReady();
    void OnFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager qnam;
};

#endif // NHTTPREQUEST_H
