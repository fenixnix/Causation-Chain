#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ecc.Generate();
    qDebug()<<ecc.GetSecKeyStr();
    qDebug()<<ecc.GetEthAddrStr();

    qDebug()<<NEcc::VerifyHash("e6e5568a0fc3537146ee3b2babfe782a851094d44b967b06d902944a1e83221955e083d79eb1a33fc2efc5c03ce0dae0096431f89d16a5b6d4f6db1e800195a0",
                    "hello!",
                    "dbc5459ffcc6f4ae03be3bae52912bcba68a1932c4c5a52655340bd92a41a77fb5b91ff4eec737a9d1b62af724a9acc801e68307c2d81a412abf8463f3fd7a06");
}
