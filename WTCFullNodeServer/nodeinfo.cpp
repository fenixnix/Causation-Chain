#include "nodeinfo.h"

NodeInfo::NodeInfo()
{

}

void NodeInfo::SetData(QString data)
{
  auto datas = data.split(',');
  id = datas[0];
  loc.Init(datas[1]);
  nat.Init(datas[2]);
  netInTime = QDateTime::currentDateTime();
  lastHeatbeatTime = netInTime;
}

void NodeInfo::HeartBeat()
{
  lastHeatbeatTime = QDateTime::currentDateTime();
}

bool NodeInfo::CheckAlive()
{
  auto t = lastHeatbeatTime.secsTo(QDateTime::currentDateTime());
  return t<=lifeCycle;
}

QString NodeInfo::ToString()
{
  QString res = id;
  res += ",";
  res+= loc.ToString();
  res+=",";
  res+= nat.ToString();
  return res;
}
