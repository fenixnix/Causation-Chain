#ifndef NODEINFO_H
#define NODEINFO_H

#include<QString>
#include<QDateTime>

class NodeInfo
{
public:
  NodeInfo();
  QString id;//blockchain address;
  QString natAddr;
  QString natPort;
  QDateTime netInTime;
  QDateTime lastHeatbeatTime;
};

#endif // NODEINFO_H
