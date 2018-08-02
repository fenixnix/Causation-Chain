#include "onnconnector.h"
#include "httprequest.h"

OnnConnector::OnnConnector(QObject *parent) : QObject(parent)
{
    connect(&ws, &NWebSocket::RcvMsg, this, &OnnConnector::RcvMsg);
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
    HttpRequest::doMethodSet(secKey,pubKey,"play");
}

void OnnConnector::SendMsg(QString msg)
{
    HttpRequest::doMethodSet(secKey,pubKey,"play",msg);
}
