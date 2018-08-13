#ifndef NQDATABASE_H
#define NQDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>

class NQDatabase : public QObject
{
  Q_OBJECT
public:
  explicit NQDatabase(QObject *parent = nullptr);
  static QStringList getSupportDatabase();

  void setDatabase(QString dbType);
  QSqlDatabase get();
  QSqlDatabase* set();
  bool open(QString dbName, QString user = "", QString pwd = "", QString host = "localhost", int port = 3306);
  void close();
  QSqlQuery* query();
  bool exec(QString sql);

signals:

public slots:

private:
  QSqlDatabase db;
  QSqlQuery* sqlQuery;
};

#endif // NQDATABASE_H
