#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "np2pringnet.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSelfTest_triggered()
{
    QSettings setting("config",QSettings::IniFormat);
    auto id = setting.value("ID").toString();
    node.setID(id);
    this->setWindowTitle(id);
    qDebug()<<"ID:"<<id;

    QIPEndPoint local(setting.value("Local").toString());
    QIPEndPoint nat(setting.value("NATServer").toString());
    QIPEndPoint p2p(setting.value("P2PServer").toString());

    node.bindLocalEndPoint(QIPEndPoint(local.IP(),local.Port()));
    node.setP2PServer(p2p);
    node.join(nat);
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
