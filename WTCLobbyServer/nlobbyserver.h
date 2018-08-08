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
    void Matching(QMap<QString, NWTCUser> &queue, int cnt);
    void RmvRoomMemberInQueue(NWTCRoom room, QMap<QString, NWTCUser> &queue);
    void GameOver(QString dat);

    QMap<QString, NWTCUser> soloQueue;
    QMap<QString, NWTCRoom*> soloRooms;
    //QList<NWTCRoom> rooms;

    QMap<QString, QTcpSocket*> clientSockets;
    NSubNet lobbyNet;

    NTcpNetwork tcp;
};

#endif // NFUNSERVER_H
