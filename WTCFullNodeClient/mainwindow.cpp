#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QObject::connect(&node,&NP2PNode::RefreshP2PmemberList,
                   this,&MainWindow::on_RefreshMemberList);
  QObject::connect(&node,&NP2PNode::RcvMsg,
                   this,&MainWindow::on_RcvMessage);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actionSelfTest_triggered()
{
  QSettings setting("config",QSettings::IniFormat);
  auto id = setting.value("ID").toString();
  node.setID(id);
  this->setWindowTitle(id);
  qDebug()<<"ID:"<<id;

  //node.SelfTest();
  QIPEndPoint local(setting.value("Local").toString());
  QIPEndPoint nat(setting.value("NATServer").toString());
  QIPEndPoint p2p(setting.value("P2PServer").toString());
  node.p2pServer.Init(p2p.IP().toString(),p2p.Port());
  node.BindLocalEndPoint(local);
  node.BindP2PServer(QIPEndPoint(local.IP(),p2p.Port()));
  node.RequireNAT(nat);
}

void MainWindow::on_actionDefault_File_triggered()
{
  QSettings setting("config",QSettings::IniFormat);
  setting.setValue("ID","Fenixnix");
  setting.setValue("Local","192.168.100.201:8421");
  setting.setValue("NATServer","118.178.127.35:8888");
  setting.setValue("P2PServer","192.168.100.204:9999");
}

void MainWindow::on_RefreshMemberList(QStringList list)
{
  ui->p2pListView->clear();
  ui->p2pListView->addItems(list);
}

void MainWindow::on_RcvMessage(QString msg)
{
  ui->plainTextEdit->appendPlainText(msg);
}

void MainWindow::on_sendBtn_clicked()
{
  QString id = ui->p2pListView->currentIndex().data().toString();
  qDebug()<<__FUNCTION__<<": "<<id<<" Msg:"<<ui->lineEdit->text();
  node.SendbyID("MSG"+ui->lineEdit->text(),id);
}
