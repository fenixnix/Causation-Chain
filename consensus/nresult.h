#ifndef NRESULT_H
#define NRESULT_H

#include <QtCore>

class NResult
{
public:
  NResult();
  quint64 timeStamp;//casue timestamp
  QString id;
  QString pubKey;
  QString data;
  QString dataHash;
};

#endif // NRESULT_H
