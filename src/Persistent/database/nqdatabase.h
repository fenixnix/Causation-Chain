#ifndef NQDATABASE_H
#define NQDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>

#define DATABASEDRIVER "QSQLITE"

class NQDatabase : public QObject
{
  Q_OBJECT
public:
  explicit NQDatabase(QObject *parent = nullptr);
  ~NQDatabase();
  static QStringList getSupportDatabase();

  bool InitSQLite();
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
  QStringList driverList;
};

#endif // NQDATABASE_H
