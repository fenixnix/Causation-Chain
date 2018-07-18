#include "nsubnet.h"

NSubNet::NSubNet()
{

}

void NSubNet::enter(QString data)
{
  NodeInfo info;
  info.SetData(data);
  if(memberList.contains(info.getId())){
      memberList[info.getId()].HeartBeat();
      return;
    }
  memberList.insert(info.getId(),info);
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

void NSubNet::heartbeat(QString id)
{
    memberList[id].HeartBeat();
}

void NSubNet::ping(QString id)
{
    memberList[id].Ping();
}

void NSubNet::pong(QString id)
{
    memberList[id].Pong();
}

NodeInfo NSubNet::get(QString id)
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

QMap<QString, NodeInfo> NSubNet::getMemberList() const
{
  return memberList;
}
