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

void MainWindow::on_btnGenKeyPair_clicked()
{
    emcc.GenerateKeyPair();
    ui->lineEditPriKey->setText(emcc.privateKeyString);
    ui->lineEditPubKey->setText(emcc.publicKeyString);
}

void MainWindow::on_btnSign_clicked()
{
    QString sign = emcc.Sign(ui->lineEditMsg->text());
    ui->EditMsgHash->setText(sign);
}

void MainWindow::on_btnVerify_clicked()
{
   auto res = NEmcc::VerifyMsg(emcc.publicKeyString,ui->lineEditMsg->text(),ui->EditMsgHash->text());
   if(res){
       ui->statusBar->showMessage("Verify Pass!!! :)");
   }else{
       ui->statusBar->showMessage("Verify Fail!!! :(");
   }
}
