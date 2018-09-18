#include "MainWindow.h"
#include <QApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QInputDialog>
#include <QTranslator>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QUdpSocket>
#include <QBuffer>
#include "HttpGet.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    QImage img("icon.png");
    QByteArray imgData;
    QBuffer imgbuffer;

    imgbuffer.open(QIODevice::WriteOnly);
    img.save(&imgbuffer,"PNG");
    imgData = imgbuffer.data().toHex();

    QByteArray imgData1 = QByteArray::fromHex(imgData);
    QImage img1 = QImage::fromData(imgData1,"PNG");
    img1.save("icon1.png");
    */
    QTranslator traslator;
    traslator.load(":/wallet.qm");
    a.installTranslator(&traslator);
    MainWindow w;
    w.show();
    return a.exec();
}
