#include "ipclassify.h"

IPClassify::IPClassify()
{

}

unsigned char IPClassify::getByte(quint32 val, int pos)
{
    return (val>>pos*8)&0xff;
}

bool IPClassify::isPrivate(QHostAddress ip)
{
    auto ipv4 = ip.toIPv4Address();
    if(ipv4>=0xA000000 && ipv4<=0xAFFFFFF){
        return true;
    }

    if(ipv4>=0xAC100000 && ipv4<=0xAC1FFFFF){
        return true;
    }

    if(ipv4>=0xC0A80000 && ipv4<=0xC0A8FFFF){
        return true;
    }
    return false;
}

bool IPClassify::isPublic(QHostAddress ip)
{
    return !isPrivate(ip);
}
