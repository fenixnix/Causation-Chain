#include <QCoreApplication>
#include "nfunserver.h"
#include "ipclassify.h"

int main(int argc, char *argv[])
{
    IPClassify::SelfTest();

    QCoreApplication a(argc, argv);
    NFunServer::SelfTest();
    qDebug()<<"Start WTC Lobby Server";
    NFunServer server;
    //NFunServer::SelfTest();

    return a.exec();
}
