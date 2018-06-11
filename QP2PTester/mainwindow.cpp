#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  udp =new QUdpSocket;
  udp->bind(8888,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
  connect(udp, &QUdpSocket::readyRead, this, &MainWindow::OnCUDP);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::OnCUDP()
{
  while(udp->hasPendingDatagrams())
    {
      QByteArray datagram;
      datagram.resize(udp->pendingDatagramSize());
      udp->readDatagram(datagram.data(), datagram.size());
      auto rcvMsg = QString::fromLatin1(datagram);
      qDebug()<<__FUNCTION__<<rcvMsg;
      ui->plainTextEdit->appendPlainText(rcvMsg);
      rcvBuffer = rcvMsg;
    }

  if(rcvBuffer.length()>0){
      QStringList list = rcvBuffer.split(",");
      for(int i = 0;i<list.size();i++){
          QStringList pair = list[i].split("-");
          if(pair.size()>=2){
              qDebug()<<"Send"<<pair[0]<<":"<<pair[1]<<":"<<ui->lineEditMsg->text();
              QUdpSocket* tUdpRcv = new QUdpSocket;
              tUdpRcv->bind(pair[1].toInt(),QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
              connect(tUdpRcv, &QUdpSocket::readyRead, this, &MainWindow::OnCUDP);
            }
        }
    }
}

//void MainWindow::OnConnect()
//{
//  qDebug()<<"WS Connected!";
//  QObject::connect(&ws, &QWebSocket::textMessageReceived,
//                   this, &MainWindow::OnWS);
//  qDebug()<<ws.localAddress().toString()<<ws.localPort();
//  qDebug()<<ws.peerAddress().toString()<<ws.peerPort()<<ws.peerName();

//  ws.sendTextMessage("WS:Hello I`m Client");
//  QString MyData="hello udpsocket";//要发送的额数据
//  udp->writeDatagram(MyData.toUtf8(),QHostAddress::LocalHost,6001);//把数据发送出去,这里的端口你可以随便写，但是要注意它没被占用
//}

//void MainWindow::OnWS(QString msg)
//{
//  qDebug()<<"Rcv WS Msg:"<<msg;
//  //emit OnMessage(msg);
//}

void MainWindow::on_pushButton_clicked()
{
  udp->writeDatagram(ui->lineEditMsg->text().toLatin1(),QHostAddress(ui->lineEditIP->text()),ui->spinBox->value());
}

void MainWindow::on_btnP2PSend_clicked()
{
    if(rcvBuffer.length()>0){
        QStringList list = rcvBuffer.split(",");
        for(int i = 0;i<list.size();i++){
            QStringList pair = list[i].split("-");
            if(pair.size()>=2){
                qDebug()<<"Send"<<pair[0]<<":"<<pair[1]<<":"<<ui->lineEditMsg->text();
                udp->writeDatagram(ui->lineEditMsg->text().toLatin1(),QHostAddress(pair[0]),pair[1].toInt());
              }
          }
      }
}
