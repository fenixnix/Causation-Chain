#ifndef P2PFULLNODENETWORK_H
#define P2PFULLNODENETWORK_H

#include <QtNetwork>
#include <QTimer>
#include <QObject>
#include <QMap>
#include <QDebug>
#include "subnet.h"

class P2PFullNodeNetwork : public QObject
{
  Q_OBJECT
public:
  explicit P2PFullNodeNetwork(QObject *parent = nullptr);
  void Init(int Port, int heartRate);
  void BoardCast(SubNet net);
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

  SubNet mainNetwork;
  QMap<QString,QIPEndPoint> peers;

  QMap<QString,SubNet> subNets;
  QUdpSocket* udp;
  //TODO: timer heartbeat
  QTimer heartbeatTimer;
};

#endif // P2PFULLNODENETWORK_H
