#include "nsubnet.h"

NSubNet::NSubNet()
{

}

void NSubNet::enter(QString data)
{
  NodeInfo info;
  info.SetData(data);
  if(memberList.contains(info.id)){
      memberList[info.id].HeartBeat();
      return;
    }
  memberList.insert(info.id,info);
}

bool NSubNet::has(QString id)
{
    return memberList.contains(id);
}

bool NSubNet::isActive(QString id)
{
    return get(id).CheckAlive();
}

void NSubNet::heartbeat(QString id)
{
    get(id).HeartBeat();
}

NodeInfo NSubNet::get(QString id)
{
    return memberList[id];
}

void NSubNet::remove(QString id)
{
    memberList.remove(id);
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
