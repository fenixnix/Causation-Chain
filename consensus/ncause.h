#ifndef NCAUSE_H
#define NCAUSE_H

#include <QtCore>

class NCause
{
public:
  NCause();
  NCause(QString json);
  NCause(quint64 ts);
  NCause(quint64 ts,QString id, QString data);

  quint64 timeStamp = 0;
  QString id = "none";
  QString pubKey = "00000000";
  QString getData() const;
  virtual void setData(const QString &value);
  void randomTestData(quint64 ts);
  QString Print();

private:
  QString data = "null";

};

#endif // NCAUSE_H
