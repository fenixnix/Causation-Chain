#include "wtcmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WTCMainWindow w;
    w.hide();
    return a.exec();
}
