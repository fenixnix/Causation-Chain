#ifndef NWTCUSER_H
#define NWTCUSER_H

#include <QString>
#include "QIPEndPoint.h"

class NWTCUser
{
public:
    NWTCUser();
    NWTCUser(QString jsonStr);
    void Init(QString jsonStr);
    void Init(QString id, QString pubKey, QString NAT, QString type);
//private:
    QString id;
    QString pubKey;
    QIPEndPoint sendEndPoint;
    QIPEndPoint nat;
    QIPEndPoint loc;

    QString type;//Solo, Team, Witness
    QString roomID;
};

#endif // NWTCUSER_H
