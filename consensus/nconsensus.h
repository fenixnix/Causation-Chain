#ifndef NCONSENSUS_H
#define NCONSENSUS_H

#include "nconsensusunit.h"
#include "nconsensusmetadata.h"

class NConsensus
{
public:
  NConsensus();
  void Add(NConsensusMetadata result);
  int maxSize();
  QString data();
  QByteArray hash();
  QString Print();

private:
  QByteArray maxSizeHash;
  QHash<QByteArray,NConsensusUnit> consensus;
  QList<QString> votedMember;
};

#endif // NCONSENSUS_H
