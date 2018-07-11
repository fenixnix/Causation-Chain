#ifndef NP2PNODE_H
#define NP2PNODE_H

#include <QtCore>
#include <QTimer>
#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QMap>
#include "qipendpoint.h"
#include "nodeinfo.h"
#include "nsubnet.h"

#define HeartBeatInterval 10

class NP2PNode : public QObject
{
    Q_OBJECT
public:
    explicit NP2PNode(QObject *parent = nullptr);
    ~NP2PNode();

    void setID(QString id);
    void bindLocalEndPoint(QIPEndPoint localEndPoint);
    void setP2PServer(QIPEndPoint server);
    void bindP2PServer(QIPEndPoint p2pServerRcv);
    void join(QIPEndPoint endPoint);

    QStringList memberList();
    //bool checkAlivebyID(QString id);
    void sendbyID(QString msg, QString id);
    void sendMsg(QString msg,QString id);
    void boardcastMsg(QString msg);

    static QHostAddress getLocalIP();
    static QString getLocalIP2();

signals:
    void P2PmemberListUpdate(QStringList list);
    void RcvMsg(QString msg);//need Sender id

private slots:
    void OnP2PServer();
    void OnNat();
    void OnHeartbeat();

private:
    void Query(QString msg);
    void RequireEnterP2PNetwork();
    void GetP2PList(QString data);
    void SendbyEndPoint(QString msg,QIPEndPoint endPoint);

    QUdpSocket* udpP2p;
    QUdpSocket* udpNat;

    QIPEndPoint natServer;
    QIPEndPoint p2pServer;
    QIPEndPoint natEndPoint;

    QString id;
    NSubNet net;
    QTimer heartbeatTimer;
};

#endif // NP2PNODE_H
