#include "onnconnector.h"
#include "httprequest.h"
#include <QSettings>

OnnConnector::OnnConnector(QObject *parent) : QObject(parent)
{
    connect(&ws, &NWsNetwork::RcvMsg, this, &OnnConnector::RcvMsg);
}

const QString cfg = "onn.cfg";

void OnnConnector::GenerateDefaultConfigFile()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    QSettings onnCfg(cfg,QSettings::IniFormat);
    onnCfg.clear();
    onnCfg.setValue("Contract","TANK3");
    onnCfg.setValue("Http","http://47.75.190.195:3000");
    //onnCfg.setValue("WebSocket","ws://47.75.190.195:3001");
    onnCfg.sync();
}

void OnnConnector::Init()
{
    if(!QFile(cfg).exists()){
        qDebug()<<"Not find: "<<cfg<<" !!";
        GenerateDefaultConfigFile();
    }
    QSettings onnCfg(cfg,QSettings::IniFormat);
    contract = onnCfg.value("Contract").toString();
    http = onnCfg.value("Http").toString();
    //QString webSocket = onnCfg.value("WebSocket").toString();
    //ws.createDataRecvWS(webSocket);
}

void OnnConnector::JoinGame(QByteArray secKey, QByteArray pubKey)
{
    this->secKey = secKey;
    this->pubKey = pubKey;
    HttpRequest::doMethodSet(secKey,pubKey,"joinGame","null",contract,http);
}

void OnnConnector::PlayGame(QString msg)
{
    HttpRequest::doMethodSet(secKey,pubKey,"play",msg.toLatin1().toHex(),contract,http);
}
