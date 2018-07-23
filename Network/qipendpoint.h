#ifndef QIPENDPOINT_H
#define QIPENDPOINT_H

#include <QtNetwork>

class QIPEndPoint
{
public:
  QIPEndPoint();
  QIPEndPoint(QString endPoint);
  QIPEndPoint(QHostAddress ip, qint16 port);
  QIPEndPoint(QString ip, qint16 port);

  void Init(QString endPoint);
  void Init(QString ip, qint16 port);
  QHostAddress IP();
  qint16 Port();
  QString ToString();

private:
  QHostAddress addr;
  qint16 port;
};

#endif // QIPENDPOINT_H
