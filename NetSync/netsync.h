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
    void Init();

    QString BoardcastBlockChainLevel(QString id, QString level);
    QString RequireBlockChainData(QString id, QString nodeAddress, QString start, QString end);
    QString SendBlockChainData(QString id, QString nodeAddress, QString data);

    void SelfTest();

signals:
    void RcvBlockChainLevel(QString id, QString nodeAddress, QString level);
    void RcvBlockChainDataRequire(QString id, QString nodeAddress, QString start, QString end);
    void RcvBlockChainData(QString id, QString nodeAddress, QString data);


public slots:

private slots:
    void RcvP2pMsg(QString signedMsg);

private:
    QString setUpSignedMsg(QString msg);

    NP2PNode p2p;
    NEmcc ecDsa;
};

#endif // NETSYNC_H
