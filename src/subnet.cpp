#include "subnet.h"

SubNet::SubNet()
{

}

void SubNet::Enter(QString data)
{
  NodeInfo info;
  info.SetData(data);
  if(memberList.contains(info.id)){
      memberList[info.id].HeartBeat();
      return;
    }
  memberList.insert(info.id,info);
}

int SubNet::GetSize()
{
  return memberList.size();
}

QString SubNet::GetMemberList()
{
  QStringList datas;
  foreach(auto n, memberList){
      datas.append(n.ToString());
    }
  return datas.join(';');
}

QMap<QString, NodeInfo> SubNet::getMemberList() const
{
  return memberList;
}
