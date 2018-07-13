#ifndef NETSYNC_H
#define NETSYNC_H

#include <QObject>
#include "np2pnode.h"
#include "nemcc.h"

class NetSync : public QObject
{
    Q_OBJECT
public:
    explicit NetSync(QObject *parent = nullptr);
    void Init(QString priKey, QString pubKey);
    void Init();
    bool PeerIsExist(QString peerAddress);
    QStringList RequirePeerList();

    void SelfTest();

signals:
    void doRcvBlockChainLevel(QString contractID, QString nodeAddress, QString level);
    void doRcvBlockChainDataRequire(QString contractID, QString nodeAddress, QString start, QString end);
    void doRcvBlockChainData(QString contractID, QString nodeAddress, QString data);
    void doUpdatePeerList(QStringList list, QStringList deadList, QStringList newComerList);
    void doRcvRequire(QString contractID, QByteArray addr, QString data);

public slots:
    void onGetBossAddr(QByteArrayList bossList);
    void onSendRequire(QString contractID, QByteArray addr, QString data);
    void onBroadcastBlockChainLevel(QString contractID, QString level);
    void onRequireBlockChainData(QString contractID, QString nodeAddress, QString start, QString end);
    void onSendBlockChainData(QString contractID, QString nodeAddress, QString data);

private slots:
    void RcvP2pMsg(QString signedMsg);
    void PeerListUpdate(QStringList list);

private:
    QString setUpSignedMsg(QString msg);
    QStringList CheckEthAddrList(QStringList list);

    QStringList prevAllPeerList;

    NP2PNode p2p;
    NEmcc ecDsa;
};

#endif // NETSYNC_H
