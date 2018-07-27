#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "np2pringnet.h"

#include <chrono>
#include <iostream>

using namespace std::chrono;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug()<<NP2PNode::getLocalIP();
    ui->setupUi(this);
    QObject::connect(&node,&NP2PNode::neighbourListUpdate,
                     this,&MainWindow::on_RefreshMemberList);

    QObject::connect(&node,&NP2PNode::RcvMsg,
                     this,&MainWindow::on_RcvMessage);

    //ui->plainTextEdit->appendPlainText(NCausationConsensus::SelfTest());
    //ui->plainTextEdit->appendPlainText(NCryptoP2P::SelfTest());
    //NCryptoP2P::SelfTest();
//    QObject::connect(&sync,&NetSync::UpdatePeerList,
//                     this,&MainWindow::on_RefreshMemberList);

    //sync.Init();
    //NP2PRingNet::SelfTest();


    //QObject::connect(&sync, &NetSync::doOnnRequire, this, &MainWindow::OnRcvOnnRequire);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSelfTest_triggered()
{
    NEmcc emcc;
    QSettings cryptoSetting("crypto.cfg", QSettings::IniFormat);
    QString secKey = cryptoSetting.value("SecKey").toString();
    QString pubKey = cryptoSetting.value("PubKey").toString();
    emcc.SetSecKey(secKey);
    emcc.SetPubKey(pubKey);
    QString addr = emcc.address;

    QSettings p2pSetting("p2p.cfg",QSettings::IniFormat);
    QIPEndPoint local(p2pSetting.value("Local").toString());
    QIPEndPoint natServer(p2pSetting.value("NATServer").toString());
    QIPEndPoint p2pServer(p2pSetting.value("P2PServer").toString());
    node.Init(addr,natServer,p2pServer,local);

    this->setWindowTitle(addr+local.ToString());
//    NEmcc emcc;
//    emcc.GenerateKeyPair();
//    sync.Init(emcc.privateKeyString, emcc.publicKeyString);
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
    node.sendMsg(ui->lineEdit->text(),id);
}

void MainWindow::on_actionRequire_All_triggered()
{
    node.RequireAllPeersList();
}

void MainWindow::on_actionRequire_NAT_triggered()
{
    QString addr1 = "1a8e521036b453cbf3e108ae6e1a6487cae1063d";
    QString addr2 = "3ee5c7f4482682c06332590dd344cb4273dfa6b7";
    QByteArrayList list;
    list.append(QByteArray::fromHex(addr1.toLatin1()));
    list.append(QByteArray::fromHex(addr2.toLatin1()));
    node.RequireNatbyAddr(list);
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
