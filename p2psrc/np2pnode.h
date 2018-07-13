#ifndef NP2PNODE_H
#define NP2PNODE_H

#include <QtCore>
#include <QTimer>
#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QMap>
#include "messageprotocol.h"
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

    void setID(QString localAddress);
    void bindLocalEndPoint(QIPEndPoint localEndPoint);
    void setP2PServer(QIPEndPoint server);
    void bindP2PServer(QIPEndPoint p2pServerRcv);
    void join(QIPEndPoint endPoint);

    QStringList neighbourList();
    //bool checkAlivebyID(QString id);
    qint64 udpSend(QIPEndPoint endPoint, QString msg);
    qint64 udpNatSend(QIPEndPoint endPoint, QString msg);
    void sendbyAddr(QString msg, QString localAddress);
    void sendMsg(QString msg,QString localAddress);
    void boardcastMsg(QString msg);

    void RequireJoin();
    void RequireAllPeersList();
    void RequireNatbyAddr(QByteArrayList addrs);

    static QHostAddress getLocalIP();
    static QString getLocalIP2();

signals:
    void neighbourListUpdate(QStringList list);
    void RcvP2PAllAddress(QStringList list);
    void RcvMsg(QString msg);//need Sender id

private slots:
    void OnP2PServer();
    void OnNat();
    void OnHeartbeat();

private:
    void Query(QString msg);
    void GetP2PList(QString data);
    void GetAllAddr(QString data);
    void GetNatbyAddr(QString data);

    void Ping(QByteArray addr);
    void Pong(QByteArray addr);

    QUdpSocket* udpP2p;
    QUdpSocket* udpNat;

    QIPEndPoint natServer;
    QIPEndPoint p2pServer;
    QIPEndPoint natEndPoint;

    QString localAddress;
    NSubNet net;
    QTimer heartbeatTimer;
};

#endif // NP2PNODE_H
