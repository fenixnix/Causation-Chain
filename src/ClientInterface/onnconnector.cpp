#include "onnconnector.h"
#include <QSettings>
#include "utility.h"

OnnConnector::OnnConnector(QThread *parent) : QThread(parent)
{

}

OnnConnector::~OnnConnector()
{
    timer->stop();
    //    timer->deleteLater();
    //    http->deleteLater();
}

const QString cfg = "onn.cfg";

void OnnConnector::GenerateDefaultConfigFile()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    QSettings onnCfg(cfg,QSettings::IniFormat);
    onnCfg.clear();
    onnCfg.setValue("Contract","TANK");
    onnCfg.setValue("Http","http://47.75.190.195:3000");
    onnCfg.sync();
}

QString OnnConnector::GetUrl(){return onnReq.Url;}
QString OnnConnector::GetContract(){return onnReq.Contract;}

void OnnConnector::Init(QByteArray secKey, QByteArray pubKey)
{
    auto filePath = QDir::currentPath() + "/" + cfg;
    if(!QFile(filePath).exists()){
        qDebug()<<"Not find: "<<filePath<<" !!";
        GenerateDefaultConfigFile();
    }
    QSettings onnCfg(filePath,QSettings::IniFormat);
    QString contractStr = onnCfg.value("Contract").toString();
    QString httpStr = onnCfg.value("Http").toString();
    qDebug()<<"Contract:"<<contractStr<<" http:"<<httpStr;
    http = new NHttpRequest();
    QObject::connect(http, &NHttpRequest::RcvMsg, this, &OnnConnector::OnRcvHttpGet, Qt::QueuedConnection);
    onnReq.Init(secKey,pubKey);
    onnReq.SetUrlContract(httpStr,contractStr);
    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, this, &OnnConnector::OnTime, Qt::QueuedConnection);
}

void OnnConnector::Post(QString cmdMsg, QString arg)
{
    http->Post(onnReq.Url, onnReq.Post(cmdMsg, arg).toLatin1());
}

void OnnConnector::JoinGame(QString jsonArgs)
{
    Post("joinGame",jsonArgs.toLatin1().toHex());
    timer->start(500);
}

void OnnConnector::PlayGame(QString msg)
{
    //StopWatch sw;
    http->Get(onnReq.Get("getPlay",msg.toLatin1().toHex()).toLatin1());
    //qDebug()<<__FUNCTION__<<sw.Count();
}

void OnnConnector::CloseGame()
{
    Post("closeGame");
}

void OnnConnector::GetState()
{
    http->Get(onnReq.Get("getStat"));
}

void OnnConnector::GetTick(int frame)
{
    //qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<frame;
    http->Get(onnReq.Get("getTick",QString::number(frame).toLatin1().toHex()));
}

void OnnConnector::OnTime()
{
    GetState();
}

void OnnConnector::OnRcvHttpGet(QString msg)
{
    qDebug()<<__FUNCTION__<<__LINE__<<msg;
    if(msg.isEmpty())return;
    auto obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    auto method = obj["method"];

    if(method == "getStat"){
        //qDebug()<<__FUNCTION__<<__LINE__;
        if(obj["stat"] == "start"){
            if(!obj.contains("data"))return;
            auto array = obj["data"].toArray();
            auto jsonString = JSON2STRING(array);
            qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<jsonString;
            emit StartGame(jsonString);
            timer->stop();
        }
    }

    if(method == "getTick"){
        auto tick  = obj["index"].toInt();
        if(obj.contains("data")){
            auto array = obj["data"].toArray();
            auto jsonString = JSON2STRING(array);
            emit Tick(tick, jsonString);
            //qDebug()<<__FUNCTION__<<__LINE__<<jsonString;
            ++tick;
        }else{
            //qDebug()<<__FUNCTION__<<__LINE__<<tick;
        }
        emit LoopTick(tick);
        //qDebug()<<__FUNCTION__<<__LINE__<<tick;
    }
}
