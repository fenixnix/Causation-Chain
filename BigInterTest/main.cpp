#include <QCoreApplication>
#include "bigintermath.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    BigInterMath::SelfTest();
    return a.exec();
}
