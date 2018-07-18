#ifndef NODEINFO_H
#define NODEINFO_H

#include <QString>
#include "qipendpoint.h"
#include <chrono>

using namespace std::chrono;

class NodeInfo
{
public:
    NodeInfo();
    NodeInfo(QString id, QIPEndPoint loc, QIPEndPoint nat);
    void SetData(QString data);
    void SetData(QString id, QIPEndPoint loc, QIPEndPoint nat);

    void Ping();
    void Pong();
    bool CheckAlive();
    QString ToString();

    QByteArray addr;

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
};

#endif // NODEINFO_H
