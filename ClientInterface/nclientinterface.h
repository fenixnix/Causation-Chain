#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>
#include <QtNetwork>
#include <ncausationconsensus.h>
#include <ncryptop2p.h>

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

public slots:
  void OnRcvCause();
  void OnRcvResult();

private:
  int fromCCausePort;
  int toCCausePort;
  int fromCResultPort;
  int toCResultPort;
  QUdpSocket udpCause;
  QUdpSocket udpResult;

  NCryptoP2P p2p;
  NCausationConsensus consensus;
};

#endif // NCLIENTINTERFACE_H
