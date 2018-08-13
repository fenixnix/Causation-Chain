#include "nsubnet.h"

NSubNet::NSubNet()
{

}

void NSubNet::enter(NPeerData info)
{
    if(memberList.contains(info.getId())){
        memberList[info.getId()].RcvPing();
        return;
    }
    memberList.insert(info.getId(),info);
    memberList[info.getId()].RcvPing();
}

void NSubNet::enter(QString data)
{
    NPeerData info;
    info.SetData(data);
    enter(info);
}

bool NSubNet::has(QString id)
{
    return memberList.contains(id);
}

bool NSubNet::isActive(QString id)
{
    return get(id).CheckAlive();
}

QStringList NSubNet::getDeadList()
{
    QStringList deadList;
    foreach(auto memberID, memberList.keys()){
        if(!isActive(memberID)){
            deadList.append(memberID);
        }
    }
    return deadList;
}

void NSubNet::ping(QString id)
{
    memberList[id].Ping();
}

void NSubNet::rcvPing(QString id)
{
    memberList[id].RcvPing();
}

void NSubNet::pong(QString id)
{
    memberList[id].Pong();
}

NPeerData NSubNet::get(QString id)
{
    return memberList[id];
}

void NSubNet::remove(QString id)
{
    memberList.remove(id);
}

void NSubNet::remove(QStringList idList)
{
    foreach(auto i, idList){
        remove(i);
    }
}

void NSubNet::removeDeadMemberAtNow()
{
    remove(getDeadList());
}

int NSubNet::GetSize()
{
    return memberList.size();
}

QString NSubNet::getMemberListString()
{
    QStringList datas;
    foreach(auto n, memberList){
        datas.append(n.ToString());
    }
    return datas.join(';');
}

QString NSubNet::getSubnetMemberListString(QStringList subMember)
{
    QStringList datas;
    foreach(auto sub, subMember){
        datas<<memberList[sub].ToString();
    }
    return datas.join(";");
}

QMap<QString, NPeerData> NSubNet::getMemberList() const
{
    return memberList;
}
