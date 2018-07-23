#include "nwtcroom.h"

NWTCRoom::NWTCRoom()
{

}

void NWTCRoom::Add(NWTCUser user)
{
    if(members.contains(user.addr)){
        return;
    }
    members.insert(user.addr,user);
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
    auto hash = GetHash()();
    foreach(var k, members.keys()){
        members[k].roomID = hash;
    }
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
        ts<<member.addr;
    }
    auto hashBr = QCryptographicHash::hash(memberList.toLatin1(),QCryptographicHash::Keccak_256);
    return QString(hashBr.toHex());
}

QString NWTCRoom::Print()
{
    QString txt;
    QTextStream ts(&txt);
    ts<<"Room:"<<GetHash()<<"\n";
    ts<<"Members:\n";
    foreach(auto m, members){
        ts<<m.addr<<"\n";
    }
    return txt;
}
