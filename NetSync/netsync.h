#ifndef NETSYNC_H
#define NETSYNC_H

#include <QObject>
#include "mainnetserver.h"
#include "nemcc.h"
#include "ipclassify.h"

class NetSync : public QObject
{
    Q_OBJECT
public:
    explicit NetSync(QObject *parent = nullptr);
    void Init(QString secKey, QString pubKey);
    void Init();
    bool PeerIsNeighbour(QString peerAddress);
    QStringList neighbourPeerList();

    void SelfTest();

signals:
    void doUpdatePeerList(QStringList list, QStringList deadList, QStringList newComerList);
    void doPeerState(QByteArrayList livePeers);
    void doOnnRequire(QString contractID, QString addr, QString cmd, QString data);

    //Deprecated Functions
    void doRcvBlockChainLevel(QString contractID, QString nodeAddress, QString level);
    void doRcvBlockChainDataRequire(QString contractID, QString nodeAddress, QString start, QString end);
    void doRcvBlockChainData(QString contractID, QString nodeAddress, QString data);
    void doRcvRequire(QString contractID, QString addr, QString data);

public slots:
    void onGetBossAddr(QByteArrayList bossList);//implement by routing
    void onQueuePeerStatebyAddr(QByteArrayList peerList);
    void onOnnRequire(QString contractID, QByteArray addr, QString cmd, QString data);
    void onOnnBroadcast(QString contractID, QString cmd, QString data);

    //Deprecated Functions
    void onBroadcastBlockChainLevel(QString contractID, QString level);
    void onSendRequire(QString contractID, QByteArray addr, QString data);
    void onRequireBlockChainData(QString contractID, QString nodeAddress, QString start, QString end);
    void onSendBlockChainData(QString contractID, QString nodeAddress, QString data);

private slots:
    void RcvP2pMsg(QString signedMsg);
    void PeerListUpdate(QStringList list);

private:
    QString setUpSignedMsg(QString msg);
    QStringList CheckEthAddrList(QStringList list);

    QStringList prevAllPeerList;

    MainNetServer p2p;
    NEmcc ecDsa;
};

#endif // NETSYNC_H
