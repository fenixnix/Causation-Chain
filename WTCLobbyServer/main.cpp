#include <QCoreApplication>
#include "nfunserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"Start WTC funciton Server";
    NFunServer server;
    //NFunServer::SelfTest();

    return a.exec();
}
