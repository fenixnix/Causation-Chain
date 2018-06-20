#ifndef NCAUSECHAIN_H
#define NCAUSECHAIN_H

#include "ncause.h"
#include <QMap>

class NCauseChain
{
public:
  NCauseChain();
  void Add(quint64 time, NCause cause);
  QMap<quint64,NCause> causes;
};

#endif // NCAUSECHAIN_H
