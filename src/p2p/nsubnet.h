#ifndef SUBNET_H
#define SUBNET_H

#include <QMap>
#include <QString>
#include "npeerdata.h"

class NSubNet
{
public:
  NSubNet();
  QString ID;//SubNet ID

  void enter(NPeerData info);
  void enter(QString data);
  bool has(QString id);
  bool isActive(QString id);
  QStringList getDeadList();
  void ping(QString id);
  void rcvPing(QString id);
  void pong(QString id);
  NPeerData get(QString id);
  void remove(QString id);
  void remove(QStringList idList);
  void removeDeadMemberAtNow();
  int GetSize();
  QString getMemberListString();
  QString getSubnetMemberListString(QStringList subMember);

  QMap<QString, NPeerData> getMemberList() const;

private:
  QMap<QString,NPeerData> memberList;
};

#endif // SUBNET_H
