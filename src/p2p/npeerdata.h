#ifndef NODEINFO_H
#define NODEINFO_H

#include <QString>
#include "qipendpoint.h"
#include <chrono>

using namespace std::chrono;

class NPeerData
{
public:
    NPeerData();
    NPeerData(QString jsonString);
    NPeerData(QJsonObject json);
    NPeerData(QString id, QIPEndPoint loc, QIPEndPoint nat);
    bool SetDataJson(QString data);
    bool SetDataJson(QJsonObject obj);
    bool SetData(QString data);
    void SetData(QString id, QIPEndPoint loc, QIPEndPoint nat);

    void Ping();
    void RcvPing();
    void Pong();
    bool CheckAlive();
    QString ToString();

    QByteArray addrID;
    QIPEndPoint loc;
    QIPEndPoint nat;

    int ping = 0;//nano sec
    int lifeCycle = 20;//sec
    QString getId() const;
    void setId(const QString &value);

    steady_clock::time_point netInTime;

private:
    QString id;//blockchain address;
    steady_clock::time_point pingTime;
    steady_clock::time_point lastUpdateTime;
};

#endif // NODEINFO_H
