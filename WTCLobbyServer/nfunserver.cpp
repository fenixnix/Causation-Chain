#include "nfunserver.h"
#include "QJsonDocument"
#include "QJsonObject"
#include <QSettings>
#include "messageprotocol.h"
#include "wtccmddefine.h"
#include "ipclassify.h"

NFunServer::NFunServer(QObject *parent) : QObject(parent)
{
    QObject::connect(&udp, &UdpNetwork::Rcv, this, &NFunServer::OnRcvMsg);
    Init();
}

void NFunServer::Init()
{
    QSettings cfg("p2p.cfg",QSettings::IniFormat);
    QString addr;
    quint16 port;
    if(cfg.contains("Local")){
        addr = cfg.value("Local").toString();
    }else{
        addr = IPClassify::getLocalIP().toString();
    }
    if(cfg.contains("Port")){
        port = cfg.value("Port").toInt();
    }else{
        port = 8900;
    }
    udp.Listen(QHostAddress(addr),port);
    tcp.InitServer(9999);
}

void NFunServer::SelfTest()
{
//    QSettings cfg("p2p.cfg",QSettings::IniFormat);
//    cfg.setValue("Local",IPClassify::getLocalIP().toString());
//    cfg.setValue("Port",8900);
//    cfg.sync();
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
    qDebug()<<__FUNCTION__<<__LINE__<<msg;
    MessageProtocol mp;
    auto cmd = mp.Decode(msg);
    if(cmd.size()==0){
        return;
    }

    if(cmd == SV_CMD_ENTER){//enter server
        EnterLobby(mp.getData(), QIPEndPoint(senderIP,senderPort));
    }

    if(cmd == SV_CMD_QUEUE_SOLO){//solo queue
        QueueSolo(mp.getData());
    }

    if(cmd == SV_CMD_GAMEOVER){//game over
        //TODO
    }
}

void NFunServer::EnterLobby(QString dat, QIPEndPoint endPoint)
{
    qDebug()<<endPoint.ToString()<<__FUNCTION__<<dat;
    NPeerData info;
    if(!info.SetData(dat)) return;
    lobbyNet.enter(info);
    peerSendAddrs.insert(info.addr,endPoint);
    qDebug()<<info.addr<<" Join Lobby";
}

void NFunServer::QueueSolo(QString dat)
{
    qDebug()<<dat<<" start solo queue";
    soloQueue.insert(dat,users[dat]);
    CheckSolo();
}

void NFunServer::CheckSolo()
{
    if(soloQueue.size()>=2){
        Matching(soloQueue,2);
    }
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
    qDebug()<<"Game Start"<<room.Print();
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

QString NFunServer::jsonUser(QString addr, QString pubKey, QString NAT, QString type)
{
    QJsonObject obj;
    obj.insert("Addr", addr);
    obj.insert("PubKey", pubKey);
    obj.insert("NAT", NAT);
    obj.insert("Type",type);
    return QString(QJsonDocument(obj).toJson());
}
