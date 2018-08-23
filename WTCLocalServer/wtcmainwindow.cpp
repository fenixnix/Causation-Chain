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
    setWindowTitle(interface.GetID()+ " " +interface.GetContract() + " " + interface.GetUrl());
}

WTCMainWindow::~WTCMainWindow()
{
    delete ui;
}

void WTCMainWindow::on_actionSend_Causue_triggered()
{

    //while(true){
    //    req.Get("http://47.75.190.195:3000/TANK10$getTick$3134$C7F836DC6145EA4A96A67B13EA0B5F2B10DD70544A69BCB565991F3BB5019CC7114EF777CCA2C36DEF0364D2EF733770FA72B389D3C348FDA7D98A7A7A5CF384");
        //QThread::sleep(1);
    //}
}

void WTCMainWindow::on_actionSend_Result_triggered()
{

}

void WTCMainWindow::on_actionRefresh_triggered()
{
    this->setWindowTitle(interface.GetID());
}

void WTCMainWindow::onP2pMsg(QString addr, QString msg)
{
    ui->plainTextEdit->appendPlainText(addr + ":" + msg);
}

void WTCMainWindow::on_actionInit_Game_triggered()
{
    interface.OnStartGame("[\"cc48ce1703f45d6aab4877659c55036edeaeb404\","
                          "\"b61f3107c1e8dd5940c35ff679ac8db91eb17704\"]");
}

int frm = 0;
void WTCMainWindow::on_tickBtn_clicked()
{
    interface.OnTick(frm);
    frm++;
}

void WTCMainWindow::on_tstTickBtn_clicked()
{
    interface.StartTestTick();
}

void WTCMainWindow::on_actionJoin_Game_triggered()
{
    //interface.JoinTank();
}

void WTCMainWindow::on_actionClose_Game_triggered()
{
    interface.CloseTank();
}

void WTCMainWindow::on_actionLoad_Game_triggered()
{
    QString fileName = QFileDialog::getOpenFileName();
    interface.LoadTankReplay(fileName);
}

void WTCMainWindow::on_actionStart_Game_triggered()
{
    interface.StartTank();
}

void WTCMainWindow::on_StepBtn_clicked()
{
    interface.OnLoad();
}
