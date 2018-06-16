#ifndef SUBNET_H
#define SUBNET_H

#include <QMap>
#include <QString>
#include "../src/nodeinfo.h"

class SubNet
{
public:
  SubNet();
  QString ID;//SubNet ID

  void Enter(QString data);
  void Remove(QString id);
  int GetSize();
  QString GetMemberList();

  QMap<QString, NodeInfo> getMemberList() const;

private:
  QMap<QString,NodeInfo> memberList;
};

#endif // SUBNET_H
