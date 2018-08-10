#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nhr.Get("http://47.75.190.195:3000/list");
}

MainWindow::~MainWindow()
{
    delete ui;
}
