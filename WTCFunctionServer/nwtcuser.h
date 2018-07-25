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
    QIPEndPoint nat;
    QIPEndPoint local;
    QString pubKey;
    QString natString;
    QString type;//Solo, Team, Witness
    QString roomID;
};

#endif // NWTCUSER_H
