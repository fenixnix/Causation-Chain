#include "onnconnector.h"
#include "httprequest.h"

OnnConnector::OnnConnector(QObject *parent) : QObject(parent)
{
    connect(&ws, &NWsNetwork::RcvMsg, this, &OnnConnector::RcvMsg);
    Init();
}

void OnnConnector::Init()
{
    ws.createDataRecvWS();
}

void OnnConnector::JoinGame(QByteArray secKey, QByteArray pubKey)
{
    this->secKey = secKey;
    this->pubKey = pubKey;
    HttpRequest::doMethodSet(secKey,pubKey);
}

void OnnConnector::SendMsg(QString msg)
{
    HttpRequest::doMethodSet(secKey,pubKey,"play",msg.toLatin1().toHex());
}
