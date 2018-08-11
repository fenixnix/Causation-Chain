#include <QCoreApplication>
#include "necc.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NEcc::SelfTest();
    return a.exec();
}
