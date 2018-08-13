#ifndef IPCLASSIFY_H
#define IPCLASSIFY_H

#include <QtNetwork>
#include <QDebug>

class IPClassify
{
public:
    IPClassify();
    static unsigned char getByte(quint32 val,int pos);
    static bool isA(QHostAddress ip);
    static bool isB(QHostAddress ip);
    static bool isC(QHostAddress ip);
    static bool isABC(QHostAddress ip);
    static bool isPrivate(QHostAddress ip);
    static bool isPublic(QHostAddress ip);
    static QHostAddress getLocalIP();
    static QString getLocalIPString();

    static void SelfTest();
};

#endif // IPCLASSIFY_H
