#include "nfunserver.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "messageprotocol.h"
#include <QSettings>

NFunServer::NFunServer(QObject *parent) : QObject(parent)
{
    Init();
}

void NFunServer::Init()
{
    udp.SetIPCPort(8900);
    QObject::connect(&udp, &UdpNetwork::Rcv, this, &NFunServer::OnRcvMsg);
}

void NFunServer::SelfTest()
{
    NFunServer server;
//    server.OnUserJoin(jsonUser("nix","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix1","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix2","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix3","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix4","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix5","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix6","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix7","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix8","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
//    server.OnUserJoin(jsonUser("nix9","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
//    server.OnUserJoin(jsonUser("nix10","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
//    server.OnUserJoin(jsonUser("nix11","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
//    server.OnUserJoin(jsonUser("nix12","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
//    server.OnUserJoin(jsonUser("nix13","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
//    server.OnUserJoin(jsonUser("nix14","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix15","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix16","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix17","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix18","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
//    server.OnUserJoin(jsonUser("nix19","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
//    server.OnUserJoin(jsonUser("nix20","123","nix;192.168.1.200:10000;192.168.1.200:10000","team"));
//    server.OnUserJoin(jsonUser("nix21","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix22","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
//    server.OnUserJoin(jsonUser("nix23","123","nix;192.168.1.200:10000;192.168.1.200:10000","solo"));
}

void NFunServer::OnRcvMsg(QString msg, QHostAddress senderIP, quint16 senderPort)
{
    MessageProtocol mp;
    auto cmd = mp.Decode(msg);
    if(cmd.size()==0){
        return;
    }

    if(cmd == "ENTR"){//enter server
        EnterLobby(mp.getData(), QIPEndPoint(senderIP,senderPort));
    }

    if(cmd == "SLQU"){//solo queue
        MatchingSolo(mp.getData(), senderIP, senderPort);
    }

    if(cmd == "GMVR"){//game over

    }

}

void NFunServer::EnterLobby(QString dat, QIPEndPoint endPoint)
{

}

void NFunServer::MatchingSolo(QString dat, QHostAddress senderIP, quint16 senderPort)
{
    NWTCUser user;
    auto datas = dat.split(';');
    user.addr = datas[0];
    user.pubKey = datas[1];
    user.nat = datas[2];
    user.type = "solo";
    user.sendEndPoint = QIPEndPoint(senderIP, senderPort);
    soloQueue.insert(user.addr,user);
    CheckSolo();
}

void NFunServer::CheckSolo()
{
    Matching(soloQueue,2);
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

void NFunServer::Matching(QHash<QString, NWTCUser> &queue, int cnt)
{
    NWTCRoom room;
    foreach(auto u, queue.values()){
        room.Add(u);
        if(room.Size()>=cnt){
            break;
        }
    }
    //soloRooms.insert(room.GetHash(),room);
    //RmvRoomMemberInQueue(room,soloQueue);
    room.AssignRoomID();
    qDebug()<<room.Print();
    //room.Start(&udp);
}

void NFunServer::GameOver(QString dat)
{
    qDebug()<<__FUNCTION__<<dat;
    //TODO::共识结果并推入主链
}

void NFunServer::RmvRoomMemberInQueue(NWTCRoom room, QHash<QString, NWTCUser> &queue)
{
    foreach(auto m, room.GetMembers()){
        queue.remove(m);
    }
}
