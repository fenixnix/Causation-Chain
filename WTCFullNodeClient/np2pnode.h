#ifndef NP2PNODE_H
#define NP2PNODE_H

#include <QtCore>
#include <QTimer>
#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QMap>
#include "../src/qipendpoint.h"
#include "../src/nodeinfo.h"

#define HeartBeatInterval 10

class NP2PNode : public QObject
{
  Q_OBJECT
public:
  explicit NP2PNode(QObject *parent = nullptr);
  ~NP2PNode();
  QHostAddress GetLocalAddress();
  void RequireNatIpEndPoint(QIPEndPoint endPoint);

  void RequireEnterP2PNetwork();

  void SelfTest();

  void SendbyEndPoint(QString msg,QIPEndPoint endPoint);
  void SendbyID(QString msg, QString id);

signals:

public slots:

private slots:
  void OnUDP();
  void OnNat();
  void OnHeartbeat();

private:
  QUdpSocket* udp;
  QUdpSocket* udpNat;
  QIPEndPoint localEndPoint;
  QIPEndPoint natEndPoint;

  QString id;
  QMap<QString,NodeInfo> p2pMemberList;

  QTimer heartbeatTimer;
};

#endif // NP2PNODE_H
