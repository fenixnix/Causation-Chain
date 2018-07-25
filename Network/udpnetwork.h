#ifndef UDPNETWORK_H
#define UDPNETWORK_H

#include <QObject>
#include <QtNetwork>

class UdpNetwork : public QObject
{
    Q_OBJECT
public:
    explicit UdpNetwork(QObject *parent = nullptr);
    void Listen(QHostAddress addr, quint16 port);
    void SetSendEndPoint(QHostAddress addr, quint16 port);
    void SetIPCPort(int port);

    bool Send(QString msg);
    bool Send(QHostAddress addr, quint16 port, msg);

signals:
    void Rcv(QString msg, QHostAddress senderIP, quint16 senderPort);

public slots:
    void OnRcv();

private:
    QUdpSocket udp;
    QHostAddress sendAddr;
    quint16 sendPort;
};

#endif // UDPNETWORK_H
