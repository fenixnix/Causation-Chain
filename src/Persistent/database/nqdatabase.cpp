#include "nqdatabase.h"
#include <QSqlError>
#include <QSqlQuery>

NQDatabase::NQDatabase(QObject *parent) : QObject(parent)
{
    driverList = getSupportDatabase();
    qDebug()<<driverList;
}

NQDatabase::~NQDatabase()
{
    db.close();
}

QStringList NQDatabase::getSupportDatabase()
{
    return QSqlDatabase::drivers();
}

bool NQDatabase::InitSQLite()
{
    if(!driverList.contains(DATABASEDRIVER)){
        qWarning()<<DATABASEDRIVER<<" not found!!";
        return false;
    }
    db = QSqlDatabase::addDatabase(DATABASEDRIVER, "causation");
    if( !open(".//causation.db"))
    {
        qWarning() << "Can`t open database";
        return false;
    }

//    exec("create table cause1 (id int primary key, data text)");
    //exec("create table result(id int primary key, data text);");

//    exec("insert into cause1 (id, data) values(1,'data')");
//    exec("insert into cause1 (id, data) values(2,'data1')");
//    exec("insert into cause1 (id, data) values(3,'data2')");

    QString select_all_sql = "select * from cause1";
    sqlQuery->prepare(select_all_sql);
    if(!sqlQuery->exec())
    {
        qDebug()<<sqlQuery->lastError();
    }
    else
    {
        while(sqlQuery->next())
        {
            int id = sqlQuery->value(0).toInt();
            QString data = sqlQuery->value(1).toString();
            qDebug()<<QString("id:%1    data:%2").arg(id).arg(data);
        }
    }

    return true;
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
