#include "nqdatabase.h"
#include <QSqlError>
#include <QSqlQuery>

NQDatabase::NQDatabase(QObject *parent) : QObject(parent)
{

}

QStringList NQDatabase::getSupportDatabase()
{
  return QSqlDatabase::drivers();
}

void NQDatabase::setDatabase(QString dbType)
{
  db = QSqlDatabase::addDatabase(dbType);
}

QSqlDatabase NQDatabase::get()
{
  return db;
}

QSqlDatabase *NQDatabase::set()
{
  return &db;
}

bool NQDatabase::open(QString dbName, QString user, QString pwd, QString host, int port)
{
  db.setDatabaseName(dbName);
  db.setUserName(user);
  db.setPassword(pwd);
  db.setHostName(host);
  db.setPort(port);
  if (!db.open())
    {
      qWarning() << "Error: Failed to connect database." << db.lastError();
      return false;
    }
  qDebug()<< "database open";
  sqlQuery = new QSqlQuery(db);
  return true;
}

void NQDatabase::close()
{
  db.close();
}

QSqlQuery *NQDatabase::query()
{
  return sqlQuery;
}

bool NQDatabase::exec(QString sql)
{
  sqlQuery->prepare(sql);
  if(!sqlQuery->exec())
    {
      qWarning() << sqlQuery->lastError();
      return false;
    }

  qDebug() << "success!";
  return true;
}
/*
  QString insert_sql = "insert into student values (?, ?, ?)";
  sql_query.prepare(insert_sql);
  sql_query.addBindValue(index);
  sql_query.addBindValue("Wang11");
  sql_query.addBindValue(2411);
  if(!sql_query.exec())
    {
      qDebug() << sql_query.lastError();
    }
  else
    {
      qDebug() << "inserted Wang!";

    }
 */
