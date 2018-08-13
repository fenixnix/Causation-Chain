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

class P2PNetwork : public QObject
{
    Q_OBJECT
public:
    explicit P2PNetwork(QObject *parent = nullptr);
    void Init(int Port, int heartRate);
    QByteArrayList getAllPeerAddrs();
    QStringList getAllPeerAddrsString();

signals:
    void UpdateMemberList();

private slots:
    void OnRcv(QString msg, QHostAddress ip, quint16 port);
    void OnBroadcast(QByteArray addr, QIPEndPoint endPoint, QString msg);
    void OnHeartbeat();

private:
    void Join(QString data,QIPEndPoint nat);
    QStringList getNodeInfoListbyAddr(QString data);

    UdpNetwork udp;
    int heartRate = 20;//sec

    NP2PRingNet ringNet;
    QHash<QByteArray, QIPEndPoint> peers;

    QTimer heartbeatTimer;
};

#endif // P2PFULLNODENETWORK_H
