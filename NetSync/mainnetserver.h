#ifndef MAINNETSERVER_H
#define MAINNETSERVER_H

#include <QObject>
#include "np2pserverinterface.h"
#include "np2pnode.h"

class MainNetServer : public QObject
{
    Q_OBJECT
public:
    explicit MainNetServer(QObject *parent = nullptr);
    void Init(QString secKey, QString pubKey);
    void Init();
    QString getID();
    void RequireJoin();
    void RequireFullNode();
    QStringList getNeighbourList();
    void BroadcastMsg(QString msg);
    void SendMsg(QString msg, QString addr);
    //QByteArrayList RequireNodeState(QByteArrayList addrs);

signals:
    void P2PListUpdate(QStringList list);//领域节点更新列表
    void RcvMsg(QString msg);
    void NodeState(QStringList list);//节点状态

public slots:

private slots:
    void OnServerMsg(QString cmd, QString dat);
    void OnP2pMsg(QString msg);
    void OnJoin();

private:
    void UpdateP2PList(QString data);
    void GetFullNode(QString data);

    NP2PServerInterface interface;
    NP2PNode p2p;
};

#endif // MAINNETSERVER_H
