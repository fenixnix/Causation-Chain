#ifndef NFUNSERVER_H
#define NFUNSERVER_H

#include <QObject>
#include <QHash>
#include "nwtcroom.h"
#include "udpnetwork.h"
#include "nsubnet.h"

class NFunServer : public QObject
{
    Q_OBJECT
public:
    explicit NFunServer(QObject *parent = nullptr);
    void Init();
    static void SelfTest();

public slots:
    void OnRcvMsg(QString msg, QHostAddress senderIP, quint16 senderPort);

private:    
    void EnterLobby(QString dat, QIPEndPoint endPoint);
    void QueueSolo(QString dat);
    void CheckSolo();
    void Matching(QHash<QString, NWTCUser> &queue, int cnt);
    void RmvRoomMemberInQueue(NWTCRoom room, QHash<QString, NWTCUser> &queue);
    void GameOver(QString dat);

    QHash<QString, NWTCUser> users;//WTC Member
    QHash<QString, NWTCUser> soloQueue;
    QHash<QString, NWTCRoom> soloRooms;

    QHash<QByteArray, QIPEndPoint> peerSendAddrs;

    static QString jsonUser(QString addr, QString pubKey, QString NAT, QString type);

    NSubNet lobbyNet;
    UdpNetwork udp;
};

#endif // NFUNSERVER_H
