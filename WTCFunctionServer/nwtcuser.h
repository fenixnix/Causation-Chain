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
    void Init(QString addr, QString pubKey, QString NAT, QString type);
//private:
    QString addr;
    QIPEndPoint sendEndPoint;
    QString pubKey;
    QString nat;
    QString type;//Solo, Team
    QString roomID;
};

#endif // NWTCUSER_H
