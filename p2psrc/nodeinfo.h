#ifndef NODEINFO_H
#define NODEINFO_H

#include<QString>
#include<QDateTime>
#include"qipendpoint.h"

class NodeInfo
{
public:
    NodeInfo();
    NodeInfo(QString id, QIPEndPoint loc, QIPEndPoint nat);
    void SetData(QString data);
    void SetData(QString id, QIPEndPoint loc, QIPEndPoint nat);
    void HeartBeat();
    bool CheckAlive();
    QString ToString();


    QByteArray addr;

    QIPEndPoint loc;
    QIPEndPoint nat;

    QDateTime netInTime;
    QDateTime lastHeatbeatTime;

    int peerLag = 0;
    int lifeCycle = 20;
    QString getId() const;
    void setId(const QString &value);

private:
    QString id;//blockchain address;
};

#endif // NODEINFO_H
