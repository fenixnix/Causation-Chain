#include "nfunserver.h"
#include "QJsonDocument"
#include "QJsonObject"

NFunServer::NFunServer(QObject *parent) : QObject(parent)
{

}

void NFunServer::SelfTest()
{
    NFunServer server;
    server.OnUserJoin(jsonUser("nix","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix1","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix2","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix3","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix4","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix5","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix6","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix7","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix8","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
    server.OnUserJoin(jsonUser("nix9","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
    server.OnUserJoin(jsonUser("nix10","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
    server.OnUserJoin(jsonUser("nix11","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
    server.OnUserJoin(jsonUser("nix12","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
    server.OnUserJoin(jsonUser("nix13","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
    server.OnUserJoin(jsonUser("nix14","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix15","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix16","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix17","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix18","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
    server.OnUserJoin(jsonUser("nix19","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
    server.OnUserJoin(jsonUser("nix20","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
    server.OnUserJoin(jsonUser("nix21","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix22","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
    server.OnUserJoin(jsonUser("nix23","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
}

void NFunServer::OnUserJoin(QString msg)
{
    NWTCUser user(msg);
    users.insert(user.addr,user);
    if(user.type == "team"){
        teamQueue.insert(user.addr, user);
    }else{
        soloQueue.insert(user.addr, user);
    }
    if(soloQueue.size()>=2){
        MatchingSolo();
    }
    if(teamQueue.size()>=4){
        MatchingTeam();
    }
}

void NFunServer::OnRcvMsg(QString msg, QHostAddress senderIP, quint16 senderPort)
{

}

QString NFunServer::jsonUser(QString addr, QString pubKey, QString NAT, QString type)
{
    QJsonObject obj;
    obj.insert("Addr", addr);
    obj.insert("PubKey", pubKey);
    obj.insert("NAT", NAT);
    obj.insert("Type",type);
    return QString(QJsonDocument(obj).toJson());
}

void NFunServer::MatchingSolo()
{
    Matching(soloQueue,2);
}

void NFunServer::MatchingTeam()
{
    Matching(teamQueue,4);
}

void NFunServer::Matching(QHash<QString, NWTCUser> &queue, int cnt)
{
    NWTCRoom room;
    foreach(auto u, queue.values()){
        room.Add(u);
        if(room.Size()>=cnt){
            break;
        }
    }
    soloRooms.insert(room.GetHash(),room);
    RmvRoomMember(room,queue);
    room.AssignRoomID();
    qDebug()<<room.Print();
}

void NFunServer::RmvRoomMember(NWTCRoom room, QHash<QString, NWTCUser> &queue)
{
    foreach(auto m, room.GetMembers()){
        queue.remove(m);
    }
}
