#ifndef RECIVER_H
#define RECIVER_H

#include <QObject>
#include "Define.h"

class Reciver : public QObject
{
    Q_OBJECT
public:
    explicit Reciver(QString pUrl);

    QWebSocket ws;
signals:
    void toWindow(QJsonDocument);
public slots:
    void onConn();
    void onText(QString);
    void onUpdBroad();
    void onUdpConn();
    void onDisconn();
    void onTimeout();
private:
    QUdpSocket socketBroad;
    bool mFlag;
    QString mUrl;
};

#endif // RECIVER_H
