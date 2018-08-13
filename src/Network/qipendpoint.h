#ifndef QIPENDPOINT_H
#define QIPENDPOINT_H

#include <QtNetwork>

class QIPEndPoint
{
public:
  QIPEndPoint();
  QIPEndPoint(QString endPoint);
  QIPEndPoint(QHostAddress ip, quint16 port);
  QIPEndPoint(QString ip, quint16 port);

  void Init(QString endPoint);
  void Init(QString ip, quint16 port);
  QHostAddress IP();
  quint16 Port();
  QString ToString();

private:
  QHostAddress addr = QHostAddress::LocalHost;
  quint16 port = 0;
};

#endif // QIPENDPOINT_H
