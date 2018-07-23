#ifndef IPCLASSIFY_H
#define IPCLASSIFY_H

#include <QtNetwork>
#include <QDebug>

class IPClassify
{
public:
    IPClassify();
    static unsigned char getByte(quint32 val,int pos);
    static bool isPrivate(QHostAddress ip);
    static bool isPublic(QHostAddress ip);
};

#endif // IPCLASSIFY_H
