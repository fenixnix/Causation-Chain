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

  void enter(NodeInfo info);
  void enter(QString data);
  bool has(QString id);
  bool isActive(QString id);
  QStringList getDeadList();
  void ping(QString id);
  void rcvPing(QString id);
  void pong(QString id);
  NodeInfo get(QString id);
  void remove(QString id);
  void remove(QStringList idList);
  void removeDeadMemberAtNow();
  int GetSize();
  QString getMemberListString();

  QMap<QString, NodeInfo> getMemberList() const;

private:
  QMap<QString,NodeInfo> memberList;
};

#endif // SUBNET_H
