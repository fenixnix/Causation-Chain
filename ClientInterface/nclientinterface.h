#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>
#include <ncausationconsensus.h>
#include <ncryptop2p.h>
#include "udpipc.h"

#define StartPort 8890

class NClientInterface : public QObject
{
  Q_OBJECT
public:
  explicit NClientInterface(QObject *parent = nullptr);
  void SetPort(int port);

  void SendCause(QString cause);
  void SendResult(QString result);//?

signals:
  void RcvCause(QString cause);
  void RcvResult(QString result);
  //void RcvResultHash(QByteArray resultHash);

public slots:
  void OnRcvLocalCause(QString msg);
  void OnRcvLocalResult(QString msg);

  void OnRcvNetCause(quint64 ts, QString addr,QString data);
  void OnRcvNetResult(quint64 ts, QString addr, QString data);

private:
  UdpIPC ipcCause;
  UdpIPC ipcResult;

  NCryptoP2P p2p;
  NCausationConsensus consensus;
};

#endif // NCLIENTINTERFACE_H
