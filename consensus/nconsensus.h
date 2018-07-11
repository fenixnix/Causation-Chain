#ifndef NCONSENSUS_H
#define NCONSENSUS_H

#include <nconsensusblock.h>
#include "nresult.h"

class NConsensus
{
public:
  NConsensus();
  void Add(NResult result);
  int maxSize();
  QString data();

  QString Print();

private:
  QByteArray maxSizeHash;
  QMap<QByteArray,NConsensusBlock> consensus;
};

#endif // NCONSENSUS_H
