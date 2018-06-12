#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QObject::connect(&node,&NP2PNode::RefreshP2PmemberList,this,&MainWindow::on_RefreshMemberList);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actionSelfTest_triggered()
{
  QSettings setting("config",QSettings::IniFormat);
  node.setID(setting.value("ID").toString());
  qDebug()<<"ID:"<<setting.value("ID").toString();

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
