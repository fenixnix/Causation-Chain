#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "np2pringnet.h"
#include "ipclassify.h"

#include <chrono>
#include <iostream>

using namespace std::chrono;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(&node,&MainNetServer::P2PListUpdate,
                     this,&MainWindow::on_RefreshMemberList);

    QObject::connect(&node,&MainNetServer::RcvMsg,
                     this,&MainWindow::on_RcvMessage);
    qDebug()<<IPClassify::getLocalIP();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSelfTest_triggered()
{
    this->setWindowTitle(node.getID());
}

void MainWindow::on_actionDefault_File_triggered()
{
    QSettings setting("config",QSettings::IniFormat);
    setting.setValue("ID","User");
    setting.setValue("Local","127.0.0.1:8421");
    setting.setValue("NATServer","118.178.127.35:8888");
    setting.setValue("P2PServer","118.178.127.35:8889");
}

void MainWindow::on_RefreshMemberList(QStringList list)
{
    ui->p2pListView->clear();
    ui->p2pListView->addItems(list);
}

void MainWindow::on_RcvMessage(QString msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

void MainWindow::on_sendBtn_clicked()
{
    QString id = ui->p2pListView->currentIndex().data().toString();
    qDebug()<<__FUNCTION__<<": "<<id<<" Msg:"<<ui->lineEdit->text();
    node.SendMsg(ui->lineEdit->text(),id);
}

void MainWindow::on_actionRequire_NAT_triggered()
{
    QString addr1 = "1a8e521036b453cbf3e108ae6e1a6487cae1063d";
    QString addr2 = "3ee5c7f4482682c06332590dd344cb4273dfa6b7";
    QByteArrayList list;
    list.append(QByteArray::fromHex(addr1.toLatin1()));
    list.append(QByteArray::fromHex(addr2.toLatin1()));
    //node.RequireNatbyAddr(list);
}

void MainWindow::on_actionOnnRequire_triggered()
{
    //auto addr = ui->lineEditAddress->text();
    //sync.onOnnRequire("ONN",QByteArray::fromHex(addr.toLatin1()),"TESTCMD","HELLO");
}

void MainWindow::OnRcvOnnRequire(QString contractID, QString addr, QString cmd, QString data)
{
    //QString txt = contractID + "," + addr + "," + cmd + "," + data;
    //qDebug()<<txt;
    //ui->plainTextEdit->appendPlainText(txt);
}


#include "nsubnet.h"
NSubNet net;
void MainWindow::on_actionStart_triggered()
{
    net.enter(NodeInfo("Red", QIPEndPoint("192.168.1.100",8000), QIPEndPoint("45.75.190.195",8000)));
}

void MainWindow::on_actionCheck_triggered()
{
    net.removeDeadMemberAtNow();
}

void MainWindow::on_actionPing_triggered()
{
    net.ping("Red");
}
