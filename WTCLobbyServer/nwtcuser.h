#ifndef NWTCUSER_H
#define NWTCUSER_H

#include <QString>
#include "qipendpoint.h"

class NWTCUser
{
public:
    NWTCUser();
    NWTCUser(QString id, QString pubKey, QString type, QTcpSocket* socket);
    void Init(QString id, QString pubKey, QString type, QTcpSocket* socket);
//private:
    QString id;
    QString pubKey;
    QTcpSocket* socket;
    QString type;//Solo, Team, Witness
    QString roomID;
};

#endif // NWTCUSER_H
