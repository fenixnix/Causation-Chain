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
    void SetP2PList(QString data);

    void setID(QString ID);//ID addr
    QString getID();const
    void setP2PServer(QIPEndPoint server);//Set P2P Server Address
    void bindLocalEndPoint(QIPEndPoint localEndPoint);//Set local EndPoint

    void join(QIPEndPoint endPoint);

    QStringList neighbourList();
    void sendbyAddr(QString msg, QString ID);
    void sendMsg(QString msg,QString ID);
    void broadcastMsg(QString msg);

    void RequireJoin();
    void RequireAllPeersList();
    void RequireNatbyAddr(QByteArrayList addrs);

    QIPEndPoint getLocalEndPoint();
    QHostAddress getLocalAddress();
    quint16 getLocalPort();

    QIPEndPoint getNatEndPoint() const;

    QString getLocalInfoString();//for p2pServer Require

signals:
    void neighbourListUpdate(QStringList list);
    //void RcvP2PAllAddress(QStringList list);
    void RcvMsg(QString msg);//need Sender id

private slots:
    void OnP2PMsg(QString cmd, QString dat);
    void OnNatMsg(QString msg);
    void OnHeartbeat();

private:
    void GetP2PList(QString data);//
    //void GetAllAddr(QString data);//

    void Ping(QString addr);
    void Pong(QString addr);

    QString ID;
    //QIPEndPoint natServer;//!!! nat server addr init
    QIPEndPoint natEndPoint;//
    UdpNetwork nat;

    NSubNet net;
    QTimer heartbeatTimer;

    NP2PServerInterface p2pServerInterface;//Rmv
};

#endif // NP2PNODE_H
