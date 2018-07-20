#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>
#include <ncausationconsensus.h>
#include <ncryptop2p.h>
#include "udpipc.h"
#include "ntimesync.h"

#define StartPort 8890

class NClientInterface : public QObject
{
  Q_OBJECT
public:
  explicit NClientInterface(QObject *parent = nullptr);
  void SetPort(int port);

//  void SendCause(QString cause);
//  void SendResult(QString result);//?

signals:
  void RcvCause(QString cause);
  void RcvResult(QString result);
  //void RcvResultHash(QByteArray resultHash);

public slots:
  void OnTick(int frameNo);
  void OnRcvLocal(QString msg);
  void OnRcvNetCause(quint64 frame, QString addr, QString data);
  void OnRcvNetResult(quint64 frame, QString addr, QString data);

private slots:
  void OnCauseTimeOut();

private:
  void BroadcastCause(QString addr, QString causeString);
  void RcvLocalCause(QString data);
  void RcvLocalResult(QString data);

  QList<QString> articipators;

  QTimer timeOut;

  UdpIPC ipc;
  NCryptoP2P p2p;
  NCausationConsensus consensus;
  NTimeSync timeSync;
};

#endif // NCLIENTINTERFACE_H
