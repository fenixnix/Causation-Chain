#ifndef NETSYNC_H
#define NETSYNC_H

#include <QObject>
#include "mainnetserver.h"
#include "ipclassify.h"

class NetSync : public QObject
{
    Q_OBJECT
public:
    explicit NetSync(QObject *parent = nullptr);
    void Init(QString addrID);
    bool PeerIsNeighbour(QString peerAddress);
    QStringList neighbourPeerList();

    void SelfTest();

signals:
    void doUpdatePeerList(QStringList list, QStringList deadList, QStringList newComerList);
    void doPeerState(QByteArrayList livePeers);
    void doOnnRequire(QString contractID, QString addr, QString cmd, QString data);

public slots:
    void onEnterSubNet(QString contractID);
    void onQuitSubNet(QString contractID);

    void onGetBossAddr(QByteArrayList bossList);//implement by routing
    void onQueuePeerStatebyAddr(QByteArrayList peerList);
    void onOnnRequire(QString contractID, QByteArray addr, QString cmd, QString data);
    void onOnnBroadcast(QString contractID, QString cmd, QString data);

private slots:
    void RcvP2pMsg(QString msg);
    void PeerListUpdate(QStringList list);

private:
    QString PackCMD(QString contractID, QString cmd, QString data);
    //QStringList CheckEthAddrList(QStringList list);
    QStringList prevAllPeerList;
    MainNetServer p2p;
};

#endif // NETSYNC_H
