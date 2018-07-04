#include <QCoreApplication>
#include "NetSync.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NetSync net;
    net.SelfTest();
    return a.exec();
}
