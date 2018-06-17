#ifndef NODEINFO_H
#define NODEINFO_H

#include<QString>
#include<QDateTime>
#include"qipendpoint.h"

class NodeInfo
{
public:
  NodeInfo();
  void SetData(QString data);
  void HeartBeat();
  bool CheckAlive();
  QString ToString();

  QString id;//blockchain address;

  QIPEndPoint loc;
  QIPEndPoint nat;

  QDateTime netInTime;
  QDateTime lastHeatbeatTime;

  int lifeCycle = 20;
};

#endif // NODEINFO_H
