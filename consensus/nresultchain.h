#ifndef NRESULTCHAIN_H
#define NRESULTCHAIN_H

#include "nresult.h"
#include "nconsensus.h"
#include <QMap>

class NResultChain
{
public:
  NResultChain();
  void Add(quint64 time, NResult res);
  int consensusSize(quint64 time);
  QString consensusData(quint64 time);
  int lastSize();
  QString lastData();

private:
  QMap<quint64,NConsensus> consensus;
  QList<quint64> timeChain;
};

#endif // NRESULTCHAIN_H
