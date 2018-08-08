#include "nwtcuser.h"

NWTCUser::NWTCUser()
{

}

NWTCUser::NWTCUser(QString id, QString pubKey, QString type, QTcpSocket *socket)
{
    Init(id, pubKey, type, socket);
}

void NWTCUser::Init(QString addr, QString pubKey, QString type, QTcpSocket *socket)
{
    this->id = addr;
    this->pubKey = pubKey;
    this->type = type;
    this->socket = socket;
}
