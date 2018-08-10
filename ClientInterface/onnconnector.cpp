#include "onnconnector.h"
#include <QSettings>
#include "utility.h"

OnnConnector::OnnConnector(QThread *parent) : QThread(parent)
{

}

OnnConnector::~OnnConnector()
{
    http->deleteLater();
    timer->stop();
    timer->deleteLater();
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

void OnnConnector::Init(QByteArray secKey, QByteArray pubKey)
{
    if(!QFile(cfg).exists()){
        qDebug()<<"Not find: "<<cfg<<" !!";
        GenerateDefaultConfigFile();
    }
    QSettings onnCfg(cfg,QSettings::IniFormat);
    QString contractStr = onnCfg.value("Contract").toString();
    QString httpStr = onnCfg.value("Http").toString();
    http = new NHttpRequest();
    QObject::connect(http, &NHttpRequest::RcvMsg, this, &OnnConnector::OnRcvHttpGet, Qt::QueuedConnection);
    onnReq.Init(secKey,pubKey);
    onnReq.SetUrlContract(httpStr,contractStr);
    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, this, &OnnConnector::OnTime, Qt::QueuedConnection);
}

void OnnConnector::JoinGame()
{
    http->Post(onnReq.Url,onnReq.Post("joinGame").toLatin1());
    timer->start(500);
}

void OnnConnector::PlayGame(QString msg)
{
    StopWatch sw;
    http->Post(onnReq.Url,onnReq.Post("Play",msg.toLatin1().toHex()).toLatin1());
    qDebug()<<__FUNCTION__<<sw.Count();
    //auto lastUpdateTime = steady_clock::now();
    //auto difT = lastUpdateTime - tp;
    //auto ping = difT.count();
    //qDebug()<<"ping:"<<(float)ping/1000000.0f<<"ms";
}

void OnnConnector::GetState()
{
    http->Get(onnReq.Get("getStat"));
}

void OnnConnector::GetTick(int frame)
{
    http->Get(onnReq.Get("getStat"));
}

void OnnConnector::StopGame()
{
    timer->stop();
}

void OnnConnector::OnTime()
{
    GetState();
}

void OnnConnector::OnRcvHttpGet(QString msg)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<msg;
    if((msg == "null")||(msg.isEmpty())){
        return;
    }

    auto obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    auto method = obj["method"];

    if(method == "getStat"){
        //qDebug()<<__FUNCTION__<<__LINE__;
        auto array = obj["data"].toArray();
        auto jsonString = JSON2STRING(array);
        emit StartGame(jsonString);
        timer->stop();
    }

    if(method == "getTick"){
        auto tick  = obj["index"].toInt();
        auto array = obj["data"].toArray();
        auto jsonString = JSON2STRING(array);
        emit Tick(tick, jsonString);
        //qDebug()<<__FUNCTION__<<__LINE__<<jsonString;
    }
}
