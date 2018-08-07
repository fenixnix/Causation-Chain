#include "nwsnetwork.h"

NWsNetwork::NWsNetwork(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer();
    connect(m_timer,&QTimer::timeout,this,&NWsNetwork::reconnect,Qt::AutoConnection);

    dataRecvWS = Q_NULLPTR;
    connectStatus = false;

}

void NWsNetwork::createDataRecvWS(QString url)
{
    this->url = url;
    if(dataRecvWS == Q_NULLPTR){
        dataRecvWS = new QWebSocket();
        connect(dataRecvWS,&QWebSocket::disconnected,this,&NWsNetwork::onDisConnected,Qt::AutoConnection);
        connect(dataRecvWS,&QWebSocket::textMessageReceived,this,&NWsNetwork::onTextReceived,Qt::AutoConnection);
        connect(dataRecvWS,&QWebSocket::connected,this,&NWsNetwork::onConnected,Qt::AutoConnection);
        qDebug()<<"create websocket to: " + url;
        dataRecvWS->open(QUrl(url));
    }
}

void NWsNetwork::reconnect()
{
    qDebug()<<"try to reconnect!";
    dataRecvWS->abort();
    dataRecvWS->open(QUrl(url));
}

void NWsNetwork::onConnected()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    qDebug()<<"DataReveive websocket is already connect!";
    connectStatus = true;
    m_timer->stop();
    qDebug()<<"Address:"<<dataRecvWS->peerAddress();
}

void NWsNetwork::onTextReceived(QString msg)
{
    emit RcvMsg(msg);
}

void NWsNetwork::onDisConnected()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    connectStatus = false;
    m_timer->start(3000);/*-<当连接失败时，触发重连计时器，设置计数周期为3秒 */
}
