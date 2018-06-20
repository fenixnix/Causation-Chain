#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  fullNetwork.Init(8889,20);
  QObject::connect(&fullNetwork,&P2PFullNodeNetwork::UpdateMemberList,this,&MainWindow::OnNewConnect);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::OnNewConnect()
{
  ui->listWidgetMainNetwork->clear();
  auto nodeList = fullNetwork.GetMainNetwrokNodes();
  int index = 0;
  foreach(auto node,nodeList){
      ui->listWidgetMainNetwork->insertItem(index,node);
      index++;
    }
}

int i = 0;
void MainWindow::on_actionAdd_random_member_triggered()
{
    QString id = "Nix" +QString::number(i);
    QString msg = id + ",1.1.1.1:100,2.2.2.2:200,0";
    fullNetwork.EnterMain(msg,QIPEndPoint());
    i++;
}
