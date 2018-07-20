#ifndef NCONSENSUS_H
#define NCONSENSUS_H

#include <nconsensusunit.h>
#include "nconsensusmetadata.h"

class NConsensus
{
public:
  NConsensus();
  void Add(NConsensusMetadata result);
  int maxSize();
  QString data();
  QString Print();

private:
  QByteArray maxSizeHash;
  QMap<QByteArray,NConsensusUnit> consensus;
};

#endif // NCONSENSUS_H
