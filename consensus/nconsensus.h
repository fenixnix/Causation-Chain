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
private:
  QString maxSizeHash;
  QMap<QString,NConsensusBlock> consensus;
};

#endif // NCONSENSUS_H
