#ifndef P2PFULLNODENETWORK_H
#define P2PFULLNODENETWORK_H

#include <QtNetwork>
#include <QTimer>
#include <QObject>
#include <QHash>
#include <QDebug>
#include "nsubnet.h"

class P2PFullNodeNetwork : public QObject
{
  Q_OBJECT
public:
  explicit P2PFullNodeNetwork(QObject *parent = nullptr);
  void Init(int Port, int heartRate);
  void EnterMain(QString data,QIPEndPoint nat);
  void BoardCast(NSubNet net);
  QStringList GetMainNetwrokNodes();
signals:
  void UpdateMemberList();
public slots:

private slots:
  void OnNetinRequire();
  void OnHeartbeat();

private:
  quint16 portNetinRequire = 8889;
  int heartRate = 20;//sec

  NSubNet mainNet;
  QHash<QString,QIPEndPoint> peers;

  QHash<QString,NSubNet> subNets;
  QUdpSocket* udp;
  //TODO: timer heartbeat
  QTimer heartbeatTimer;
};

#endif // P2PFULLNODENETWORK_H
