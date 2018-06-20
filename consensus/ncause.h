#ifndef NCAUSE_H
#define NCAUSE_H

#include <QtCore>

class NCause
{
public:
  NCause();
  quint64 timeStamp;
  QString id;
  QString data;
  QString pubKey;
};

#endif // NCAUSE_H
