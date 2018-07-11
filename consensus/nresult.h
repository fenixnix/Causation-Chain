#ifndef NRESULT_H
#define NRESULT_H

#include <QtCore>
#include <ncause.h>

class NResult : public NCause
{
public:
  NResult();
  NResult(QString json);
  NResult(quint64 ts, QString id, QString data);
  NResult(quint64 ts, QString id, QString pubKey, QString data);
  NResult(quint64 ts);

//  quint64 timeStamp;//casue timestamp
//  QString id;
//  QString pubKey;
//  QString data;
  void setData(const QString &value);
  QByteArray dataHash;

  QString Print();
};

#endif // NRESULT_H
