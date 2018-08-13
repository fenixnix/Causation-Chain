#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  qDebug()<<NQDatabase::getSupportDatabase();
  db.setDatabase("QMYSQL3");
  db.open("trace_schema_en","maintain","1234","192.168.100.188",3306);
  //db.open("test","Nix","qazwsx");
}

MainWindow::~MainWindow()
{
  db.close();
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  QString create_sql = "create table Inspection (id int primary key, reslut MEDIUMTEXT)";
  db.exec(create_sql);
}

int index = 0;
void MainWindow::on_pushButton_3_clicked()
{
  auto query = db.query();
  QString insert_sql = "INSERT INTO Inspection VALUES(?, ?)";
  query->prepare(insert_sql);
  query->addBindValue(index);
  query->addBindValue("abcdefgh");
  if(!query->exec())
    {
      qDebug() << query->lastError();
    }
  else
    {
      qDebug() << "inserted";

    }
  index ++;
  //  if(!sql_query.exec("INSERT INTO student VALUES(3, \"Li\", 23)"))
  //  {
  //      qDebug() << sql_query.lastError();
  //  }
  //  else
  //  {
  //      qDebug() << "inserted Li!";
  //  }
}


#include <QSqlTableModel>
void initializeModel(QSqlTableModel *model) {

  model->setTable("Inspection");

  model->setEditStrategy(QSqlTableModel::OnRowChange);

  model->select();

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));

  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Data"));

}



// 使用model view

QTableView *createView(const QString &title, QSqlTableModel *model) {

  QTableView *view = new QTableView;

  view->setModel(model);

  view->setWindowTitle(title);

  return view;

}

void MainWindow::on_pushButton_4_clicked()
{
//  QString select_all_sql = "select * from Inspection";
//  if(db.exec(select_all_sql)){
//      auto query = db.query();
//      while(query->next()){
//          int id = query->value(0).toInt();
//          QString name = query->value(1).toString();

//          qDebug()<<QString("id:%1    data:%2 ").arg(id).arg(name);
//        }
//    }

  QSqlTableModel *model = new QSqlTableModel(this, db.get());
       model->setTable("Inspection");
       model->setEditStrategy(QSqlTableModel::OnManualSubmit);
       model->select();
       model->setHeaderData(0, Qt::Horizontal, tr("ID"));
       model->setHeaderData(1, Qt::Horizontal, tr("Data"));

  ui->tableView->setModel(model);
  ui->tableView->hideColumn(0);

}
