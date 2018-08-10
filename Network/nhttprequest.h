#ifndef NHTTPREQUEST_H
#define NHTTPREQUEST_H

#include <QObject>
#include <QtNetwork>

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

private:
    QNetworkAccessManager qnam;
};

#endif // NHTTPREQUEST_H
