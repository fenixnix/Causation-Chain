#include "onnconnector.h"
#include "httprequest.h"
#include <QSettings>
#include <chrono>

using namespace std::chrono;

OnnConnector::OnnConnector(QObject *parent) : QObject(parent)
{
    connect(&httpGet, &HttpGet::doGetData, this, &OnnConnector::OnRcvHttpGet);
    connect(&timer, &QTimer::timeout, this, &OnnConnector::OnTime);
}

const QString cfg = "onn.cfg";

void OnnConnector::GenerateDefaultConfigFile()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    QSettings onnCfg(cfg,QSettings::IniFormat);
    onnCfg.clear();
    onnCfg.setValue("Contract","TANK4");
    onnCfg.setValue("Http","http://47.75.190.195:3000");
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
}

void OnnConnector::JoinGame(QByteArray secKey, QByteArray pubKey)
{
    this->secKey = secKey;
    this->pubKey = pubKey;
    HttpRequest::doMethodSet(secKey,pubKey,"joinGame","null",contract,http);
    timer.start(500);
}

void OnnConnector::PlayGame(QString msg)
{
    //auto tp = steady_clock::now();
    HttpRequest::doMethodSet(secKey,pubKey,"play",msg.toLatin1().toHex(),contract,http);
    //auto lastUpdateTime = steady_clock::now();
    //auto difT = lastUpdateTime - tp;
    //auto ping = difT.count();
    //qDebug()<<"ping:"<<(float)ping/1000000.0f<<"ms";
}

void OnnConnector::GetState()
{
    httpGet.onGet(QUrl(HttpRequest::doMethodGet(
                        pubKey,"getStat","null",contract,http)));
}

void OnnConnector::GetTick(int frame)
{
    httpGet.onGet(QUrl(HttpRequest::doMethodGet(
                        pubKey,"getTick",QByteArray::number(frame).toHex(),contract,http)));
}

void OnnConnector::StopGame()
{
    timer.stop();
}

void OnnConnector::OnTime()
{
    GetState();
}

void OnnConnector::OnRcvHttpGet(QString msg)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<msg;
    if(msg == "null"){
        return;
    }

    auto obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    auto method = obj["method"];

    if(method == "getStat"){
        auto array = obj["data"].toArray();
        auto jsonString = QString(QJsonDocument(array).toJson(QJsonDocument::Compact));
        emit StartGame(jsonString);
        timer.stop();
    }

    if(method == "getTick"){
        auto tick  = obj["index"].toInt();
        auto array = obj["data"].toArray();
        auto jsonString = QString(QJsonDocument(array).toJson(QJsonDocument::Compact));
        emit Tick(tick, jsonString);
        //qDebug()<<__FUNCTION__<<__LINE__<<jsonString;
    }
}
