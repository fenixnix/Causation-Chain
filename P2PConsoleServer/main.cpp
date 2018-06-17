#include <QCoreApplication>
#include "p2pfullnodenetwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    P2PFullNodeNetwork network;
    network.Init(8889,20);
    return a.exec();
}
