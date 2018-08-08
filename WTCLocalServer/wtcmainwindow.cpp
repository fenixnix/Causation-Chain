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
    ui->listWidget->clear();
    ui->listWidget->addItems(interface.GetMemberList());
    this->setWindowTitle(interface.getID());
}

void WTCMainWindow::on_sendMsgBtn_clicked()
{
    interface.SendChatMsg(ui->listWidget->currentIndex().data().toString(),ui->lineEditMsg->text());
}

void WTCMainWindow::onP2pMsg(QString addr, QString msg)
{
    ui->plainTextEdit->appendPlainText(addr + ":" + msg);
}

void WTCMainWindow::on_actionEnter_Lobby_triggered()
{
    interface.Enter_Lobby();
}

void WTCMainWindow::on_actionStart_Solo_triggered()
{
    interface.Queue_Solo();
}

void WTCMainWindow::on_actionJoin_Tank_triggered()
{
    interface.JoinTank();
}

void WTCMainWindow::on_actionInit_Game_triggered()
{
    QString id = QInputDialog::getText(this,"ID","ID");
    QString jsonString = "{\"method\":\"startGame\",\"owner\":\"";
    jsonString += id;
    jsonString += "\",\"msg\":[\"P11\",\"P22\",\"P33\"],\"result\":true,\"symbol\":\"TANK\"}";
    interface.SendGameInitInfo(jsonString);
}
