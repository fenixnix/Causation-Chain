#ifndef SUBNET_H
#define SUBNET_H

#include <QMap>
#include <QString>
#include "nodeinfo.h"

class NSubNet
{
public:
  NSubNet();
  QString ID;//SubNet ID

  void enter(QString data);
  bool has(QString id);
  bool isActive(QString id);
  void heartbeat(QString id);
  NodeInfo get(QString id);
  void remove(QString id);
  int GetSize();
  QString getMemberListString();


  QMap<QString, NodeInfo> getMemberList() const;

private:
  QMap<QString,NodeInfo> memberList;
};

#endif // SUBNET_H
