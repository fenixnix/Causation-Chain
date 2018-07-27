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
#include "np2pserverinterface.h"

#define HeartBeatInterval 10

class NP2PNode : public QObject
{
    Q_OBJECT
public:
    explicit NP2PNode(QObject *parent = nullptr);
    ~NP2PNode();
    void Init(QString id, QIPEndPoint natServer, QIPEndPoint p2pServer, QIPEndPoint local);

    void setID(QString localAddress);//ID addr
    void setP2PServer(QIPEndPoint server);//Set P2P Server Address
    void bindLocalEndPoint(QIPEndPoint localEndPoint);//Set local EndPoint

    void join(QIPEndPoint endPoint);

    QStringList neighbourList();
    void sendbyAddr(QString msg, QString localAddress);
    void sendMsg(QString msg,QString localAddress);
    void broadcastMsg(QString msg);

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
    void OnP2PMsg(QString cmd, QString dat);
    void OnNatMsg(QString msg);
    void OnHeartbeat();

private:
    void GetP2PList(QString data);
    void GetAllAddr(QString data);
    void GetNatbyAddr(QString data);

    void Ping(QString addr);
    void Pong(QString addr);

    //QIPEndPoint natServer;//!!! nat server addr init
    QIPEndPoint natEndPoint;//

    UdpNetwork nat;
    NP2PServerInterface p2pServerInterface;
    QString localAddress;
    NSubNet net;
    QTimer heartbeatTimer;
};

#endif // NP2PNODE_H
