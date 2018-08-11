#include "wtcmainwindow.h"
#include "ui_wtcmainwindow.h"

WTCMainWindow::WTCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WTCMainWindow)
{
    ui->setupUi(this);
    //interface.StartTest();
    //NCmdPacker::SelfTest();
    //ui->plainTextEdit->appendPlainText(NCausationConsensus::SelfTest());
    QObject::connect(&interface, &NClientInterface::RcvMsg, this, &WTCMainWindow::onP2pMsg);
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

void WTCMainWindow::on_actionRefresh_triggered()
{
    this->setWindowTitle(interface.getID());
}

void WTCMainWindow::onP2pMsg(QString addr, QString msg)
{
    ui->plainTextEdit->appendPlainText(addr + ":" + msg);
}

void WTCMainWindow::on_actionJoin_Tank_triggered()
{
    interface.JoinTank();
}

void WTCMainWindow::on_actionInit_Game_triggered()
{
    interface.OnStartGame("[\"83ff259f5e5eb75738e1fb65e7f5212b444c6d00\",\"User2\"]");
}

int frm = 0;
void WTCMainWindow::on_tickBtn_clicked()
{
    interface.OnTick(frm);
    frm++;
}
