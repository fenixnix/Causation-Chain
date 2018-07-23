#include "nfunserver.h"
#include "QJsonDocument"
#include "QJsonObject"

NFunServer::NFunServer(QObject *parent) : QObject(parent)
{

}

void NFunServer::OnUserJoin(QString msg)
{
    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    auto addr = obj["Addr"].toString();
    auto pubKey = obj["PubKey"].toString();
    auto nat = obj["NAT"].toString();

}
