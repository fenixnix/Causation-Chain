#ifndef NCAUSE_H
#define NCAUSE_H

#include <QtCore>

class NConsensusMetadata
{
public:
  NConsensusMetadata();
  NConsensusMetadata(QString json);
  NConsensusMetadata(quint64 ts);
  NConsensusMetadata(quint64 ts,QString addr, QString data);

  quint64 frame = 0;
  QString addr = "none";
  QByteArray hash;

  QString getData() const;
  void setData(const QString &value);

  void randomTestData(quint64 ts);
  QString Print();

private:
  QString data = "null";

};

#endif // NCAUSE_H
