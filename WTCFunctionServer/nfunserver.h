#ifndef NFUNSERVER_H
#define NFUNSERVER_H

#include <QObject>
#include <QHash>
#include "nwtcroom.h"
#include "udpipc.h"

class NFunServer : public QObject
{
    Q_OBJECT
public:
    explicit NFunServer(QObject *parent = nullptr);
    static void SelfTest();
signals:

public slots:
    void OnUserJoin(QString msg);

private:
    QHash<QString, NWTCUser> users;
    QHash<QString, NWTCUser> soloQueue;
    QHash<QString, NWTCUser> teamQueue;
    QHash<QString, NWTCRoom> rooms;

    static QString jsonUser(QString addr, QString pubKey, QString NAT, QString type);

    void MatchingSolo();
    void MatchingTeam();
    void Matching(QHash<QString, NWTCUser> &queue, int cnt);
    void RmvRoomMember(NWTCRoom room, QHash<QString, NWTCUser> &queue);

    UdpIPC ipc;
};

#endif // NFUNSERVER_H
