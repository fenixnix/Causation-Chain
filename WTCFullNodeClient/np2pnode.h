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

  void setID(QString id);
  void BindLocalEndPoint(QIPEndPoint localEndPoint);
  void BindP2PServer(QIPEndPoint p2pServer);
  void RequireNAT(QIPEndPoint endPoint);
  void RequireEnterP2PNetwork();

  void Query(QString msg);

  void SendbyEndPoint(QString msg,QIPEndPoint endPoint);
  void SendbyID(QString msg, QString id);

  bool CheckAlivebyID(QString id);

  QIPEndPoint p2pServer;
  QIPEndPoint natServer;

  void SelfTest();

signals:

public slots:

private slots:
  void OnP2PServer();
  void OnNat();
  void OnHeartbeat();

private:
  QUdpSocket* udpP2p;
  QUdpSocket* udpNat;

  QIPEndPoint natEndPoint;

  QString id;
  QMap<QString,NodeInfo> p2pMemberList;

  QTimer heartbeatTimer;
};

#endif // NP2PNODE_H
