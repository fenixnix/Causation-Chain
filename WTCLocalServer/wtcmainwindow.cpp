#include "wtcmainwindow.h"
#include "ui_wtcmainwindow.h"

WTCMainWindow::WTCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WTCMainWindow)
{
    ui->setupUi(this);
    NCmdPacker::SelfTest();
    ui->plainTextEdit->appendPlainText(NCausationConsensus::SelfTest());
}

WTCMainWindow::~WTCMainWindow()
{
    delete ui;
}

void WTCMainWindow::on_actionSend_Causue_triggered()
{

}

void WTCMainWindow::on_actionSend_Result_triggered()
{

}
