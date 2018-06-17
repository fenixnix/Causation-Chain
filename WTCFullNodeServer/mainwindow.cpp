#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  fullNetwork.Init(8889,20);
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
