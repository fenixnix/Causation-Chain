#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QObject::connect(&fullNetwork,&P2PFullNodeNetwork::NewConnect,this,&MainWindow::OnNewConnect);
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

void MainWindow::on_actionSelfTest_triggered()
{
  for(int i = 0;i<10;i++){
      ui->listWidgetMainNetwork->insertItem(i,QString::number(i));
    }
}
