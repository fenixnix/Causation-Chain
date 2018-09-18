#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    const QString cryptoFileName = "crypto.cfg";
    QSettings cryptoSetting(cryptoFileName, QSettings::IniFormat);
    if(!QFile(cryptoFileName).exists()){
        qDebug()<<"Not find crypto.cfg, generate new keyPair!!";
        ecc.Generate();
        cryptoSetting.setValue("SecKey", ecc.privateKeyString);
        cryptoSetting.setValue("PubKey", ecc.publicKeyString);
        cryptoSetting.sync();
    }
    QString secKey = cryptoSetting.value("SecKey").toString();
    QString pubKey = cryptoSetting.value("PubKey").toString();
    ecc.SetKey(secKey, pubKey);
    onn.Init(secKey.toLatin1(),pubKey.toLatin1());


    auto filePath = QDir::currentPath() + "/" + "onn.cfg";
    if(!QFile(filePath).exists()){
        qDebug()<<"Not find: "<<filePath<<" !!";
        GenerateDefaultConfigFile();
    }
    QSettings onnCfg(filePath,QSettings::IniFormat);
    QString contractStr = onnCfg.value("Contract").toString();
    QString httpStr = onnCfg.value("Http").toString();
    qDebug()<<"Contract:"<<contractStr<<" http:"<<httpStr;
    http = new NHttpRequest();
    QObject::connect(http, &NHttpRequest::RcvMsg, this, &MainWindow::OnRcvInfo, Qt::QueuedConnection);

    onn.SetUrlContract(httpStr,contractStr);
//    timer = new QTimer();
//    QObject::connect(timer, &QTimer::timeout, this, &OnnConnector::OnTime, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetInfo(QString addr)
{
    http->Post(onn.Url,onn.Get("GetInfo",addr));
}

void MainWindow::on_actionRecharge_triggered()
{
    QString addr;
    int amount = 0;
    http->Post(onnReq.Url,onn.Post("Recharge",addr + "|" + QString::number(amount)));
}

void MainWindow::on_pushButton_clicked()
{
    http->Post(onnReq.Url,onn.Post("BuyHashRate"));
}

void MainWindow::OnRcvInfo(QString msg)
{

}
