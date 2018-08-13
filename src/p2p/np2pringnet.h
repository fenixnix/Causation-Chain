#ifndef NP2PPUBLICNETWORK_H
#define NP2PPUBLICNETWORK_H

#include <QObject>
#include <QStringList>
#include <QLinkedList>
#include <QHash>
#include <npeerdata.h>

class NP2PRingNet : public QObject
{
    Q_OBJECT
public:
    explicit NP2PRingNet(QObject *parent = nullptr);

    void peerJoinCall(NPeerData peerInfo);
    void update();

    QByteArrayList getAllAddress();
    NPeerData getNodeInfo(QByteArray addr);

    static void SelfTest();

signals:
    void Send(QByteArray addr, QIPEndPoint endPoint, QString msg);

public slots:

private:
    QString getPeersNeighbourList(QLinkedList<QByteArray>::iterator itr);
    void broadcastNeighberPeersList();
    void clearDeadPeers();

    int neighbourRange = 3;
    QLinkedList<QByteArray> peerAddrList;
    QHash<QByteArray,NPeerData> peerInfoHashMap;
    QList<QByteArray> deadPeers;

    QLinkedList<QByteArray>::iterator next(QLinkedList<QByteArray>::iterator iitr);
    QLinkedList<QByteArray>::iterator prev(QLinkedList<QByteArray>::iterator iitr);
};

/*
 *环状列表地址
 *节点向服务发送心跳包来维持节点存在
 *服务在列表中定时广播领域节点信息，环前后10个节点
 *
*/
#endif // NP2PPUBLICNETWORK_H
