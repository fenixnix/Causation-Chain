#ifndef TCPNETWORK_H
#define TCPNETWORK_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include <QHash>
#include "qipendpoint.h"

class NTcpNetwork : public QObject
{
    Q_OBJECT
public:
    explicit NTcpNetwork(QObject *parent = nullptr);
    void InitServer(quint16 port);
    void InitClient(QIPEndPoint ep);
    void Send2Server(QString msg);
    void Broadcast(QString msg);
    static void SendMsg(QTcpSocket* socket, QString msg);
signals:
    void ServerRcvMsg(QString msg, QTcpSocket* sender);
    void ClientRcvMsg(QString msg);
    void ClientDisconnect(QTcpSocket* sender);

private slots:
    void OnNewConnect();
    void OnClientDisconnect();
    void OnServerRcvMsg();
    void OnClientRcvMsg();

private:
    QTcpServer server;
    QList<QTcpSocket*> connections;
    QHash<QByteArray, QTcpSocket*> clientConnections;

    QTcpSocket client;
};

#endif // TCPNETWORK_H
