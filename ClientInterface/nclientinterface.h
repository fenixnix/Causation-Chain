#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>
#include "ncausationconsensus.h"
#include "ncmdpacker.h"
#include "ncryptop2p.h"
#include "udpipc.h"
#include "ntimesync.h"

#define StartPort 8890

class NClientInterface : public QObject
{
  Q_OBJECT
public:
  explicit NClientInterface(QObject *parent = nullptr);
  void SetPort(int port);

signals:
  void RcvCause(QString cause);
  void RcvResult(QString result);
  //void RcvResultHash(QByteArray resultHash);

public slots:
  void OnTick(int frameNo);
  void OnRcvLocal(QString msg);
  void OnRcvNet(quint64 timeStamp, QString addr, QString msg);

private slots:
  void OnCauseTimeOut();

private:
  void BroadcastCause();
  void RcvLocalCause(QString data);
  void RcvLocalResult(QString data);

  void RcvNetCause(quint64 frame, QString addr, QString data);
  void RcvNetCausePack(quint64 frame, QString addr, QString data);
  void RcvNetResult(quint64 frame, QString addr, QString data);

  QList<QString> articipators;

  QTimer timeOut;

  UdpIPC ipc;
  NCryptoP2P p2p;
  NCmdPacker packer;
  NCausationConsensus consensus;
  NTimeSync timeSync;
};

#endif // NCLIENTINTERFACE_H
