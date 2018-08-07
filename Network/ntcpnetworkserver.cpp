#include "ntcpnetworkserver.h"

NTcpNetworkServer::NTcpNetworkServer(QObject *parent) : QObject(parent)
{
    connect(&server, &QTcpServer::newConnection, this, &NTcpNetworkServer::OnNewConnect);
    //connect(&server, &QTcpServer::acceptError, this, &NTcpNetworkServer::OnNewConnect);
}

void NTcpNetworkServer::Init()
{
    auto ret = server.listen(QHostAddress::LocalHost,8889);
    if(!ret){
        qDebug()<<__FUNCTION__<<server.errorString();
    }
}

void NTcpNetworkServer::OnNewConnect()
{
    while(server.hasPendingConnections()){
        QTcpSocket* clientConnection = server.nextPendingConnection();
    }
}
