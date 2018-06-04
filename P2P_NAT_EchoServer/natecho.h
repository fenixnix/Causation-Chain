#ifndef NATECHO_H
#define NATECHO_H

#include <QtNetwork>
#include <QObject>

#define UDPPORT 8888

class NatEcho : public QObject
{
  Q_OBJECT
public:
  explicit NatEcho(QObject *parent = nullptr);

signals:

public slots:

private slots:
  void OnUDP();

private:
  QUdpSocket* udp;
};

#endif // NATECHO_H
