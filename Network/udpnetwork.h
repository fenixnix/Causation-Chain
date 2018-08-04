#ifndef UDPNETWORK_H
#define UDPNETWORK_H

#include <QObject>
#include <QtNetwork>
#include "qipendpoint.h"

class UdpNetwork : public QObject
{
    Q_OBJECT
public:
    explicit UdpNetwork(QObject *parent = nullptr);
    ~UdpNetwork();
    void Listen(quint16 port);
    void Listen(QIPEndPoint endPoint);
    void Listen(QHostAddress addr, quint16 port);
    void SetSendEndPoint(QHostAddress addr, quint16 port);
    void SetIPCPort(int port);
    void Close();

    bool Send(QString msg);
    bool Send(QHostAddress addr, quint16 port, QString msg);
    bool Send(QIPEndPoint ep, QString msg);
    bool SendAndSet(QHostAddress addr, quint16 port, QString msg);

    QHostAddress getSendAddr() const;
    quint16 getSendPort() const;
    QHostAddress getRcvAddr() const;
    quint16 getRcvPort() const;
    QIPEndPoint getRcvEndPoint() const;

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
