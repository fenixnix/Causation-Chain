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
#include "npeerdata.h"
#include "nsubnet.h"
#include "np2pserverinterface.h"

#define HeartBeatInterval 3

class NP2PNode : public QObject
{
    Q_OBJECT
public:
    explicit NP2PNode(QObject *parent = nullptr);
    ~NP2PNode();
    void Init(QString id, QIPEndPoint natServer, QIPEndPoint local);
    void SetP2PList(QString data);

    void setID(QString ID);//ID addr
    QString getID() const;

    void bindLocalEndPoint(QIPEndPoint localEndPoint);//Set local EndPoint
    void join(QIPEndPoint ep);

    QStringList neighbourList();
    void sendMsg(QString msg,QString ID);
    void broadcastMsg(QString msg);

    QIPEndPoint getLocalEndPoint();
    QHostAddress getLocalAddress();
    quint16 getLocalPort();

    QIPEndPoint getNatEndPoint() const;

    QString getLocalInfoString();//for p2pServer Require

signals:
    void RequireJoin();
    void neighbourListUpdate(QStringList list);
    //void RcvP2PAllAddress(QStringList list);
    void RcvMsg(QString msg);//need Sender id

private slots:
    void OnNatMsg(QString msg);
    void OnHeartbeat();

private:
    void Ping(QString addr);
    void Pong(QString addr);
    void sendbyAddr(QString msg, QString ID);

    QString ID;
    QIPEndPoint natServer;//!!! nat server addr init
    QIPEndPoint natEndPoint;//
    UdpNetwork nat;

    NSubNet net;
    QTimer heartbeatTimer;
};

#endif // NP2PNODE_H
