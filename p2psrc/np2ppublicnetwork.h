#ifndef NP2PPUBLICNETWORK_H
#define NP2PPUBLICNETWORK_H

#include <QObject>
#include <QLinkedList>
#include <QHash>
#include <nodeinfo.h>

class NP2PPublicNetwork : public QObject
{
    Q_OBJECT
public:
    explicit NP2PPublicNetwork(QObject *parent = nullptr);

//    QLinkedList<NodeInfo> peers;
//    QLinkedList<NodeInfo>::iterator iPtr;
//    QLinkedList<NodeInfo>::iterator next();
//    QLinkedList<NodeInfo>::iterator prev();

    void newPeerJoin();
    void boardNeighberPeersList();
    bool checkPeerLife(QByteArray addr);
    void clearDeadPeers();

    QLinkedList<QString> peers;
    QList<QByteArray> deadPeers;
    QLinkedList<QString>::iterator iPtr;
    QLinkedList<QString>::iterator next();
    QLinkedList<QString>::iterator prev();
    //QHash<>

    static void SelfTest();
signals:

public slots:

private:
    int neighberRange = 10;
    //iterator next();
    //iterator prev();
};

/*
 *环状列表地址
 *节点向服务发送心跳包来维持节点存在
 *服务在列表中定时广播领域节点信息，环前后10个节点
 *
*/
#endif // NP2PPUBLICNETWORK_H
