#ifndef NETSYNC_H
#define NETSYNC_H

#include <QObject>
#include "np2pnode.h"
#include "nemcc.h"

//哪些部分写需求哪些节点列表

//新进入Diff
//领域节点列表
//全网节点列表
//死亡节点列表本地运算
//新进入的列表本地运算

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
    void RcvBlockChainLevel(QString id, QString nodeAddress, QString level);
    void RcvBlockChainDataRequire(QString id, QString nodeAddress, QString start, QString end);
    void RcvBlockChainData(QString id, QString nodeAddress, QString data);
    void UpdatePeerList(QStringList list, QStringList deadList, QStringList newComerList);

public slots:
    void BroadcastBlockChainLevel(QString id, QString level);
    void RequireBlockChainData(QString id, QString nodeAddress, QString start, QString end);
    void SendBlockChainData(QString id, QString nodeAddress, QString data);

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
