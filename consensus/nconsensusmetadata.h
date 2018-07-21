#ifndef NCAUSE_H
#define NCAUSE_H

#include <QtCore>

class NConsensusMetadata
{
public:
  NConsensusMetadata();
  NConsensusMetadata(QString json);
  NConsensusMetadata(quint64 f);
  NConsensusMetadata(quint64 f,QString addr, QString data);
  NConsensusMetadata(quint64 f,QString addr, QString data, QByteArray hash);

  quint64 frame = 0;
  QString addr;
  QByteArray hash;

  QString getData() const;
  void setData(const QString &value);

  void randomTestData(quint64 f);
  QString Print();

private:
  QString data = "null";

};

#endif // NCAUSE_H
