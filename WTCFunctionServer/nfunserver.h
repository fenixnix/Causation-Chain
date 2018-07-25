#ifndef NFUNSERVER_H
#define NFUNSERVER_H

#include <QObject>
#include <QHash>
#include "nwtcroom.h"
#include "udpnetwork.h"

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
    void MatchingSolo(QString dat, QHostAddress senderIP, quint16 senderPort);
    void CheckSolo();
    void Matching(QHash<QString, NWTCUser> &queue, int cnt);
    void RmvRoomMemberInQueue(NWTCRoom room, QHash<QString, NWTCUser> &queue);
    void GameOver(QString dat);

    QHash<QString, NWTCUser> users;//WTC Member
    QHash<QString, NWTCUser> soloQueue;
    //QHash<QString, NWTCUser> teamQueue;
    QHash<QString, NWTCRoom> soloRooms;

    static QString jsonUser(QString addr, QString pubKey, QString NAT, QString type);

    UdpNetwork udp;
};

#endif // NFUNSERVER_H
