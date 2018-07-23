#ifndef UDPIPC_H
#define UDPIPC_H

#include <QObject>
#include <QtNetwork>

class UdpIPC : public QObject
{
    Q_OBJECT
public:
    explicit UdpIPC(QObject *parent = nullptr);
    void SetPort(int port);
    void Send(QString msg);
signals:
    void Rcv(QString msg);
public slots:
    void OnRcv();
private slots:

private:
    int sendPort;
    int rcvPort;
    QUdpSocket udp;
};

#endif // UDPIPC_H
