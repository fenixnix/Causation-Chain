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
    void Init();
    void RequireJoin();
    void RequireFullNode();
    //void RequireNode();

signals:
    void P2PListUpdate(QStringList list);//领域节点更新列表
    void NodeState(QStringList list);//节点状态

public slots:

private slots:
    void OnServerMsg(QString cmd, QString dat);

private:
    void UpdateP2PList(QString data);
    void GetFullNode(QString data);

    NP2PServerInterface interface;
    NP2PNode p2p;
};

#endif // MAINNETSERVER_H
