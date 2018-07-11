#ifndef NCAUSECHAIN_H
#define NCAUSECHAIN_H

#include "ncause.h"
#include <QMap>

class NCauseChain
{
public:
  NCauseChain();
  bool Add(NCause cause);
  QMap<quint64,NCause> causes;
  quint64 lastTimeStamp = 0;

  QString Print();

  static QString SelfTest();
};

#endif // NCAUSECHAIN_H
