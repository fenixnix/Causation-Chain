#include "nodeinfo.h"

NodeInfo::NodeInfo()
{

}

void NodeInfo::SetData(QString data)
{
  auto datas = data.split(',');
  if(data.size()<3){
      return;
  }
  id = datas[0];
  loc.Init(datas[1]);
  nat.Init(datas[2]);
  netInTime = QDateTime::currentDateTime();
  lastHeatbeatTime = netInTime;
}

void NodeInfo::HeartBeat()
{
  //qDebug()<<__FUNCTION__<<lastHeatbeatTime.toString();
  lastHeatbeatTime = QDateTime::currentDateTime();
  //qDebug()<<__FUNCTION__<<1<<lastHeatbeatTime.toString();
}

bool NodeInfo::CheckAlive()
{
  auto t = lastHeatbeatTime.secsTo(QDateTime::currentDateTime());
  //qDebug()<<__FUNCTION__<<t<<lastHeatbeatTime.toString();
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
