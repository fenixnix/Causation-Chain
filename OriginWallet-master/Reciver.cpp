#include "Reciver.h"

QTimer *timerRC = new QTimer();

Reciver::Reciver(QString pUrl){
    mFlag = false;
    mUrl = pUrl;
    connect(&ws,SIGNAL(connected()),this,SLOT(onConn()),Qt::QueuedConnection);
    connect(&ws,SIGNAL(disconnected()),this,SLOT(onDisconn()),Qt::QueuedConnection);
    connect(&ws,SIGNAL(textMessageReceived(QString)),this,SLOT(onText(QString)),Qt::QueuedConnection);
    connect(&socketBroad,SIGNAL(connected()),this,SLOT(onUdpConn()),Qt::QueuedConnection);
    connect(&socketBroad,SIGNAL(readyRead()),this,SLOT(onUpdBroad()),Qt::QueuedConnection);
    connect(timerRC,SIGNAL(timeout()),this,SLOT(onTimeout()),Qt::QueuedConnection);
    ws.open(QUrl(QString("ws://")+pUrl));
    BUG << QString("ws://")+pUrl;
    timerRC->start(5000);
}

void Reciver::onConn(){
    mFlag = true;
    BUG << ws.peerName() << ws.peerAddress() << ws.peerPort();
    BUG << ws.localAddress() << ws.localPort();
    //socketBroad.connectToHost("118.178.127.35",ws.localPort());
    //socketBroad.connectToHost(QHostAddress("58.33.80.115"),ws.localPort());
    //socketBroad.bind(QHostAddress("58.33.80.115"), ws.localPort());
}

void Reciver::onDisconn(){
    mFlag = false;
}

void Reciver::onTimeout(){
    if(!mFlag){
        ws.close();
        ws.open(QUrl(QString("ws://")+mUrl));
    }
}

void Reciver::onUdpConn(){
    BUG << socketBroad.peerAddress() << socketBroad.peerPort();
    QHostAddress recvAddress = QHostAddress("192.168.1.6");
    quint16 recvPort = ws.localPort();
    //socketBroad.joinMulticastGroup(QHostAddress("58.33.80.115"));

    while(1){
        QByteArray data;
        BUG << socketBroad.readDatagram(data.data(),1000,&recvAddress,&recvPort);
        BUG << data << data.count();
        BUG << socketBroad.receiveDatagram(1000).data();
        QThread::sleep(1);
    }

}

void Reciver::onText(QString pMsg){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(pMsg.toLatin1());
    //QJsonArray    jsonArr = jsonDoc.array();
    BUG << jsonDoc;
    emit toWindow(jsonDoc);
}

void Reciver::onUpdBroad(){
    BUG << socketBroad.readAll();
}
