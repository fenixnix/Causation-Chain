#ifndef MAINNETSERVER_H
#define MAINNETSERVER_H

#include <QObject>
#include "np2pserverinterface.h"

class MainNetServer : public QObject
{
    Q_OBJECT
public:
    explicit MainNetServer(QObject *parent = nullptr);
    void Init();

signals:
    void P2PListUpdate(QStringList list);//领域节点更新列表
    void FullNodeUpdate(QStringList list);//全节点列表
    void NodeState(QStringList list);//节点状态

public slots:

private slots:
    void OnServerMsg(QString cmd, QString dat);

private:
    NP2PServerInterface interface;
    void UpdateP2PList(QString data);
    void GetFullNode(QString data);
};

#endif // MAINNETSERVER_H
