#include "nwtcuser.h"

NWTCUser::NWTCUser()
{

}

NWTCUser::NWTCUser(QString jsonStr)
{
    Init(jsonStr);
}

void NWTCUser::Init(QString jsonStr)
{
    QJsonObject obj = QJsonDocument::fromJson(jsonStr.toLatin1()).object();
    auto addr = obj["Addr"].toString();
    auto pubKey = obj["PubKey"].toString();
    auto nat = obj["NAT"].toString();
    auto type = obj["Type"].toString();
    Init(addr,pubKey,nat,type);
}

void NWTCUser::Init(QString addr, QString pubKey, QString NAT, QString type)
{
    this->id = addr;
    this->pubKey = pubKey;
    this->nat.Init(NAT);
    this->type = type;
    auto datas = NAT.split(';');
    sendEndPoint = QIPEndPoint(datas[1]);
}
