#ifndef P2PFULLNODENETWORK_H
#define P2PFULLNODENETWORK_H

#include <QtNetwork>
#include <QTimer>
#include <QObject>
#include <QHash>
#include <QDebug>
#include "messageprotocol.h"
#include "nsubnet.h"
#include "np2pringnet.h"
#include "udpnetwork.h"

class P2PFullNodeNetwork : public QObject
{
    Q_OBJECT
public:
    explicit P2PFullNodeNetwork(QObject *parent = nullptr);
    void Init(int Port, int heartRate);
    QByteArrayList getAllPeerAddrs();
    QStringList getAllPeerAddrsString();

signals:
    void UpdateMemberList();

public slots:

private slots:
    void OnRcv(QString msg, QHostAddress ip, quint16 port);
    //void OnNetRequire();
    void OnHeartbeat();
    void OnBroadcast(QByteArray addr, QIPEndPoint endPoint, QString msg);

private:
    void EnterMain(QString data,QIPEndPoint nat);
    QStringList getNodeInfoListbyAddr(QString data);

    quint16 portNetinRequire = 8889;
    int heartRate = 20;//sec

    NP2PRingNet ringNet;
    QHash<QByteArray, QIPEndPoint> peers;

    UdpNetwork udp;

    //QUdpSocket* udp;
    //qint64 udpSend(QIPEndPoint endPoint, QString msg);

    //TODO: timer heartbeat
    QTimer heartbeatTimer;
};

#endif // P2PFULLNODENETWORK_H
