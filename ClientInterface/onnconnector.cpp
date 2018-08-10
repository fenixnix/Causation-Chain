#include "onnconnector.h"
#include "httprequest.h"
#include <QSettings>
#include <chrono>

using namespace std::chrono;

OnnConnector::OnnConnector(QThread *parent) : QThread(parent)
{
    QObject::connect(&timer, &QTimer::timeout, this, &OnnConnector::OnTime, Qt::QueuedConnection);
    timer.start(500);
}

const QString cfg = "onn.cfg";

void OnnConnector::GenerateDefaultConfigFile()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    QSettings onnCfg(cfg,QSettings::IniFormat);
    onnCfg.clear();
    onnCfg.setValue("Contract","TANK9");
    onnCfg.setValue("Http","http://47.75.190.195:3000");
    onnCfg.sync();
}

void OnnConnector::Init()
{
    qDebug()<<this->thread();
    if(!QFile(cfg).exists()){
        qDebug()<<"Not find: "<<cfg<<" !!";
        GenerateDefaultConfigFile();
    }
    QSettings onnCfg(cfg,QSettings::IniFormat);
    contract = onnCfg.value("Contract").toString();
    httpGet = new HttpGet();
    QObject::connect(httpGet, &HttpGet::doGetData, this, &OnnConnector::OnRcvHttpGet, Qt::QueuedConnection);
    QObject::connect(this, &OnnConnector::doHttpGet, httpGet, &HttpGet::onGet, Qt::QueuedConnection);
    http = onnCfg.value("Http").toString();
}

void OnnConnector::JoinGame(QByteArray secKey, QByteArray pubKey)
{
    this->secKey = secKey;
    this->pubKey = pubKey;
    HttpRequest::doMethodSet(secKey,pubKey,"joinGame","null",contract,http);
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
    //qDebug()<<__FUNCTION__<<__LINE__;
    emit doHttpGet(QUrl(HttpRequest::doMethodGet(
                        pubKey,"getStat","null",contract,http)));
}

void OnnConnector::GetTick(int frame)
{
    emit doHttpGet(QUrl(HttpRequest::doMethodGet(
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
        //qDebug()<<__FUNCTION__<<__LINE__;
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
