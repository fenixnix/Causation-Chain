#include "nwebsocket.h"

NWebSocket::NWebSocket(QObject *parent) : QObject(parent)
{
    dataRecvWS = Q_NULLPTR;
    connectStatus = false;
    m_timer = new QTimer();
}

void NWebSocket::createDataRecvWS()
{
    if(dataRecvWS == Q_NULLPTR){
        dataRecvWS = new QWebSocket();
        qDebug()<<"create websocket!";
        connect(dataRecvWS,&QWebSocket::disconnected,this,DataReceive::onDisConnected,Qt::AutoConnection);
        connect(dataRecvWS,&QWebSocket::textMessageReceived,this,DataReceive::onTextReceived,Qt::AutoConnection);
        connect(dataRecvWS,&QWebSocket::connected,this,DataReceive::onConnected,Qt::AutoConnection);
        connect(m_timer,m_timer->timeout,this,DataReceive::reconnect,Qt::AutoConnection);
        dataRecvWS->open(QUrl("ws://192.168.0.17:60000/data"));
    }
}

void NWebSocket::reconnect()
{
    qDebug()<<"try to reconnect!";
    dataRecvWS->abort();
    dataRecvWS->open(QUrl("ws://192.168.0.17:60000/data"));
}

void NWebSocket::onConnected()
{
    qDebug()<<"DataReveive websocket is already connect!";
    connectStatus = true;
    m_timer->stop();
    qDebug()<<"Address："<<dataRecvWS->peerAddress();
}

void NWebSocket::onTextReceived(QString msg)
{
    qDebug()<<"----------------data-----------------";
    qDebug()<<msg;
}

void NWebSocket::onDisConnected()
{
    qDebug()<<"DataReceive websocket is disconnected!!!";
    connectStatus = false;
    m_timer->start(3000);/*-<当连接失败时，触发重连计时器，设置计数周期为3秒 */
}
