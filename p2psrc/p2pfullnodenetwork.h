#ifndef P2PFULLNODENETWORK_H
#define P2PFULLNODENETWORK_H

#include <QtNetwork>
#include <QTimer>
#include <QObject>
#include <QMap>
#include <QDebug>
#include "nsubnet.h"

class P2PFullNodeNetwork : public QObject
{
  Q_OBJECT
public:
  explicit P2PFullNodeNetwork(QObject *parent = nullptr);
  void Init(int Port, int heartRate);
  void BoardCast(NSubNet net);
  QStringList GetMainNetwrokNodes();
signals:
  void NewConnect();
public slots:

private slots:
  void OnNetinRequire();
  void OnHeartbeat();

private:
  quint16 portNetinRequire = 8889;
  int heartRate = 20;//sec

  NSubNet mainNet;
  QMap<QString,QIPEndPoint> peers;

  QMap<QString,NSubNet> subNets;
  QUdpSocket* udp;
  //TODO: timer heartbeat
  QTimer heartbeatTimer;
};

#endif // P2PFULLNODENETWORK_H
