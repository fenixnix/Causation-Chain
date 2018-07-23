#ifndef NWTCUSER_H
#define NWTCUSER_H

#include <QString>
#include "QIPEndPoint.h"

class NWTCUser
{
public:
    NWTCUser();
    void Init(QString jsonStr);
private:
    QString addr;
    QIPEndPoint sendEndPoint;
    QString pubKey;
    QString nat;
};

#endif // NWTCUSER_H
