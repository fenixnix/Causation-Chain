#ifndef P2PFULLNODENETWORK_H
#define P2PFULLNODENETWORK_H

#include <QtNetwork>
#include <QTimer>
#include <QObject>
#include <QHash>
#include <QDebug>
#include "nsubnet.h"
#include "np2pringnet.h"

#define CMDSIZE 4

class P2PFullNodeNetwork : public QObject
{
  Q_OBJECT
public:
  explicit P2PFullNodeNetwork(QObject *parent = nullptr);
  void Init(int Port, int heartRate);
  QByteArrayList getAllPeerAddrs();
  QStringList getAllPeerAddrsString();

signals:
  void UpdateMemberList();

public slots:

private slots:
  void OnNetRequire();
  void OnHeartbeat();
  void OnBroadcast(QByteArray addr, QIPEndPoint endPoint, QString msg);

private:
  void EnterMain(QString data,QIPEndPoint nat);

  quint16 portNetinRequire = 8889;
  int heartRate = 20;//sec

  NP2PRingNet ringNet;
  QHash<QByteArray, QIPEndPoint> peers;

  QUdpSocket* udp;
  qint64 udpSend(QIPEndPoint endPoint, QString msg);

  //TODO: timer heartbeat
  QTimer heartbeatTimer;
};

#endif // P2PFULLNODENETWORK_H
