#include "ntcpnetwork.h"

NTcpNetwork::NTcpNetwork(QObject *parent) : QObject(parent)
{
    connect(&server, &QTcpServer::newConnection, this, &NTcpNetwork::OnNewConnect);
    //connect(&server, &QTcpServer::acceptError, this, &NTcpNetworkServer::OnNewConnect);

    connect(&client, &QTcpSocket::readyRead, this, &NTcpNetwork::OnClientRcvMsg);
}

void NTcpNetwork::InitServer(quint16 port)
{
    auto ret = server.listen(QHostAddress::LocalHost,port);
    if(!ret){
        qDebug()<<__FUNCTION__<<server.errorString();
    }
}

void NTcpNetwork::InitClient(QIPEndPoint ep)
{
    client.abort();
    client.connectToHost(ep.IP(),ep.Port());
}

void NTcpNetwork::Send2Server(QString msg)
{
    SendMsg(&client,msg);
}

void NTcpNetwork::OnNewConnect()
{
    while(server.hasPendingConnections()){
        QTcpSocket* clientConnection = server.nextPendingConnection();
        auto senderAddr = clientConnection->peerAddress().toString();
        qDebug()<<__FUNCTION__<<senderAddr;
        clientConnections.insert(senderAddr.toLatin1(),clientConnection);
        connections<<clientConnection;
        connect(clientConnection, &QTcpSocket::readyRead, this, &NTcpNetwork::OnServerRcvMsg);
        SendMsg(clientConnection,"Hello");
    }
}

void NTcpNetwork::OnServerRcvMsg()
{
    QTcpSocket* clientSocket = (QTcpSocket*)this->sender();
    QString dat = QString(clientSocket->readAll());
    qDebug()<<__FUNCTION__<<dat;
    emit ServerRcvMsg(clientSocket, dat);
}

void NTcpNetwork::OnClientRcvMsg()
{
    auto data =  client.readAll();
    qDebug()<<__FUNCTION__<<QString(data);
    SendMsg(&client,"Hello too");
    emit ClientRcvMsg(QString(data));
}

void NTcpNetwork::SendMsg(QTcpSocket *socket, QString msg)
{
    socket->write(msg.toLatin1());
}
