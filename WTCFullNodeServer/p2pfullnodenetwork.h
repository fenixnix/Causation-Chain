#ifndef P2PFULLNODENETWORK_H
#define P2PFULLNODENETWORK_H

#include <QtNetwork>
#include <QObject>
#include <QMap>
#include <QDebug>
#include "subnet.h"

class P2PFullNodeNetwork : public QObject
{
  Q_OBJECT
public:
  explicit P2PFullNodeNetwork(QObject *parent = nullptr);
  void BoardCast(SubNet net);
  QStringList GetMainNetwrokNodes();
signals:
  void NewConnect();
public slots:

private slots:
  void OnNetinRequire();

private:
  quint16 portNetinRequire = 9999;
  int life = 20;//sec
  SubNet mainNetwork;
  QMap<QString,SubNet> subNets;
  QUdpSocket* udp;
};

#endif // P2PFULLNODENETWORK_H
