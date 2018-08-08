#include "nlobbyserver.h"
#include "QJsonDocument"
#include "QJsonObject"
#include <QSettings>
#include "messageprotocol.h"
#include "wtccmddefine.h"
#include "ipclassify.h"

NLobbyServer::NLobbyServer(QObject *parent) : QObject(parent)
{
    connect(&tcp, &NTcpNetwork::ServerRcvMsg, this, &NLobbyServer::OnRcvMsg);
}

void NLobbyServer::Init(quint16 port)
{
    tcp.InitServer(port);
}

void NLobbyServer::SelfTest()
{
    NLobbyServer server;
    //    server.OnUserJoin(jsonUser("nix2","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
}

void NLobbyServer::OnRcvMsg(QString msg, QTcpSocket *socket)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<msg;
    MessageProtocol mp;
    auto cmd = mp.Decode(msg);
    if(cmd.size()==0){
        return;
    }

    if(cmd == SV_CMD_ENTER){//enter server
        EnterLobby(mp.getData(), socket);
    }

    if(cmd == SV_CMD_QUEUE_SOLO){//solo queue
        QueueSolo(mp.getData());
    }

    if(cmd == SV_CMD_GAMEOVER){//game over
        //TODO
    }
}

void NLobbyServer::EnterLobby(QString dat, QTcpSocket *socket)
{
    qDebug()<<__FUNCTION__<<dat;
    NPeerData info;
    if(!info.SetData(dat)) return;
    lobbyNet.enter(info);
    clientSockets.insert(info.getId(), socket);
    qDebug()<<info.addr<<" Join Lobby";
}

void NLobbyServer::QueueSolo(QString dat)
{
    qDebug()<<dat<<" start solo queue";
    auto pair = dat.split(';');
    NWTCUser user(pair[0],pair[1],"solo",clientSockets[pair[0]]);
    soloQueue.insert(user.id,user);
    CheckSolo();
}

void NLobbyServer::CheckSolo()
{
    if(soloQueue.size()>=2){
        Matching(soloQueue,2);
    }
}

void NLobbyServer::Matching(QHash<QString, NWTCUser> &queue, int cnt)
{
    NWTCRoom room;
    foreach(auto u, queue.values()){
        room.Add(u);
        if(room.Size()>=cnt){
            break;
        }
    }
    room.AssignRoomID();

    qDebug()<<__FUNCTION__<<"Game Start"<<room.Print();
    auto memberList = room.GetMembers();
    auto subNetP2pMsg = SV_CMD_LOBBY_NET + lobbyNet.getSubnetMemberListString(memberList);
    foreach(auto member, memberList){
        NTcpNetwork::SendMsg(clientSockets[member],subNetP2pMsg);
    }

    //room.Start(&tcp);
}

void NLobbyServer::GameOver(QString dat)
{
    qDebug()<<__FUNCTION__<<dat;
    //TODO::共识结果并推入主链
}

void NLobbyServer::RmvRoomMemberInQueue(NWTCRoom room, QHash<QString, NWTCUser> &queue)
{
    foreach(auto m, room.GetMembers()){
        queue.remove(m);
    }
}
