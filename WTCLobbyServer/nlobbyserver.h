#ifndef NFUNSERVER_H
#define NFUNSERVER_H

#include <QObject>
#include <QHash>
#include <QTimer>
#include "udpnetwork.h"
#include "ntcpnetwork.h"
#include "nsubnet.h"
#include "nwtcroom.h"

class NLobbyServer : public QObject
{
    Q_OBJECT
public:
    explicit NLobbyServer(QObject *parent = nullptr);
    void Init(quint16 port);
    static void SelfTest();

public slots:
    void OnRcvMsg(QString msg, QTcpSocket* socket);

private:    
    void EnterLobby(QString dat, QTcpSocket* socket);
    void QueueSolo(QString dat);
    void CheckSolo();
    void Matching(QHash<QString, NWTCUser> &queue, int cnt);
    void RmvRoomMemberInQueue(NWTCRoom room, QHash<QString, NWTCUser> &queue);
    void GameOver(QString dat);

    QHash<QString, NWTCUser> soloQueue;
    QHash<QString, NWTCRoom> soloRooms;

    QHash<QString, QTcpSocket*> clientSockets;
    NSubNet lobbyNet;

    NTcpNetwork tcp;
};

#endif // NFUNSERVER_H
