#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>
#include <QtNetwork>

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
  int toCCausePort = 8890;
  int toCResultPort = 8891;
  int fromCCausePort = 8892;
  int fromCResultPort = 8893;
  QUdpSocket udpCause;
  QUdpSocket udpResult;
};

#endif // NCLIENTINTERFACE_H
