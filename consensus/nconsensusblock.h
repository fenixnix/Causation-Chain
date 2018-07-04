#ifndef NCONSENSUSBLOCK_H
#define NCONSENSUSBLOCK_H

#include <QtCore>

class NConsensusBlock
{
public:
  NConsensusBlock();
  NConsensusBlock(QString hash, QString data);
  void AddMember(QString id);
  int Size();
  QString datahash;
  QString data;
  QList<QString> members;
};

#endif // NCONSENSUSBLOCK_H
