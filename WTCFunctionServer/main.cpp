#include <QCoreApplication>
#include "nfunserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NFunServer::SelfTest();
    return a.exec();
}
