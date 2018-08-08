#include "nwtcroom.h"
#include "wtccmddefine.h"

NWTCRoom::NWTCRoom(QObject *parent) : QObject(parent)
{

}

void NWTCRoom::Add(NWTCUser user)
{
    if(members.contains(user.id)){
        return;
    }
    members.insert(user.id,user);
}

int NWTCRoom::Size()
{
    return members.size();
}

bool NWTCRoom::IsExist(QString addr)
{
    return members.contains(addr);
}

void NWTCRoom::AssignRoomID()
{
    auto hash = GetHash();
    foreach(auto k, members.keys()){
        members[k].roomID = hash;
    }
}

void NWTCRoom::Start(NTcpNetwork *tcp)
{
    this->tcp = tcp;
    QJsonArray memberArray;
    foreach (auto m, members) {
        QJsonObject user;
        user["Addr"] = m.id;
        user["PubKey"] = m.pubKey;
        memberArray.append(user);
    }
    QJsonObject obj;
    obj["Members"] = memberArray;
    obj["RoomID"] = roomID;

    //game start
//    QString msg = "GMST" + QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));

//    foreach(auto m, members){
//        tcp->Send(m.sendEndPoint.IP(),m.sendEndPoint.Port(),msg);
//    }

    QObject::connect(&timer, &QTimer::timeout, this, &NWTCRoom::OnTrig);
    timer.start(3000);
}

QStringList NWTCRoom::GetMembers()
{
    return members.keys();
}

QString NWTCRoom::GetHash()
{
    QString memberList;
    QTextStream ts(&memberList);
    foreach(auto member, members){
        ts<<member.id;
    }
    auto hashBr = QCryptographicHash::hash(memberList.toLatin1(),QCryptographicHash::Keccak_256);
    roomID = QString(hashBr.toHex());
    return roomID;
}

QString NWTCRoom::Print()
{
    QString txt;
    QTextStream ts(&txt);
    ts<<"Room:"<<GetHash()<<"\n";
    ts<<"Members:\n";
    foreach(auto m, members){
        ts<<m.id<<"\n";
    }
    return txt;
}

void NWTCRoom::OnTrig()
{
    qDebug()<<__FUNCTION__;
    QJsonObject obj;
    obj["Room:"] = roomID;
    QString msg = SV_CMD_TICK + QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    foreach(auto m, members){
        NTcpNetwork::SendMsg(m.socket,msg);
    }
}
