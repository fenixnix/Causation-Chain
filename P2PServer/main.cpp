#include <QCoreApplication>
#include "p2pnetwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //过滤D，E类地址，127.0.0.1，Unable send 之后，直接Kill地址
    P2PNetwork network;
    network.Init(8889,20);
    return a.exec();
}
