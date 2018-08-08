#include <QCoreApplication>
#include "nlobbyserver.h"
#include "ipclassify.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //NLobbyServer::SelfTest();
    qDebug()<<"Start WTC Lobby Server";
    NLobbyServer server;
    server.Init(9999);
    return a.exec();
}
