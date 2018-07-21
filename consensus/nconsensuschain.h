#ifndef NRESULTCHAIN_H
#define NRESULTCHAIN_H

#include "nconsensusmetadata.h"
#include "nconsensus.h"
#include <QMap>

class NConsensusChain
{
public:
  NConsensusChain();
  void Add(NConsensusMetadata res);
  int consensusSize(quint64 frame);
  QString consensusData(quint64 frame);
  QByteArray consensusHash(quint64 frame);
  int lastSize();
  QString lastData();
  QString lastHash();
  quint64 lastFrame = 0;

  QString Print();

  static QString SelfTest();

private:
  QMap<quint64,NConsensus> consensus;
  QList<quint64> frameChain;
};

#endif // NRESULTCHAIN_H
