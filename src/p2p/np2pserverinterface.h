#ifndef NP2PSERVERINTERFACE_H
#define NP2PSERVERINTERFACE_H

#include <QObject>
#include "udpnetwork.h"

class NP2PServerInterface : public QObject
{
    Q_OBJECT
public:
    explicit NP2PServerInterface(QObject *parent = nullptr);
    void Init(QIPEndPoint local, QIPEndPoint server);
    void Init(QHostAddress local, QHostAddress server, quint16 port);
    void Query(QString msg);

signals:
    void ServerMsg(QString cmd, QString dat);

private slots:
    void OnUdp(QString msg);

private:
    UdpNetwork udp;
};

#endif // NP2PSERVERINTERFACE_H
