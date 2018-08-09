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

QString OnnConnector::GetState()
{
    emit doGet(QUrl(HttpRequest::doMethodGet(
                        pubKey,"getStat","null",contract,http)));
}

QString OnnConnector::GetTick(int frame)
{
    emit doGet(QUrl(HttpRequest::doMethodGet(
                        pubKey,"getTick",QByteArray::number(frame).toHex(),contract,http)));
}

void OnnConnector::StopGame()
{
    timer.stop();
}

void OnnConnector::OnTime()
{
    auto res = GetState();
    if(res!="null"){
        emit StartGame(res);
        timer.stop();
    }
}
