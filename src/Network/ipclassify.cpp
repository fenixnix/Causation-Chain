#include "ipclassify.h"
#include <QDebug>

IPClassify::IPClassify()
{

}

unsigned char IPClassify::getByte(quint32 val, int pos)
{
    return (val>>pos*8)&0xff;
}

bool IPClassify::isA(QHostAddress ip)
{
    auto ipv4 = ip.toIPv4Address();
    return ipv4>=0x100001 && ipv4 <= 0x7EFFFFFE;
}

bool IPClassify::isB(QHostAddress ip)
{
    auto ipv4 = ip.toIPv4Address();
    return ipv4>=0x80010001 && ipv4 <= 0xBFFEFFFE;
}

bool IPClassify::isC(QHostAddress ip)
{
    auto ipv4 = ip.toIPv4Address();
    return ipv4>=0xC000101 && ipv4 <= 0xDFFFFEFE;
}

bool IPClassify::isABC(QHostAddress ip)
{
    return isA(ip)||isB(ip)||isC(ip);
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
    if(isABC(ip)){
        return !isPrivate(ip);
    }
    return false;
}

QHostAddress IPClassify::getLocalIP()
{
    auto hostName = QHostInfo::localHostName();
    auto host = QHostInfo::fromName(hostName);
    foreach(auto ip, host.addresses()){
        if((ip.protocol() == QAbstractSocket::IPv4Protocol)&&(isABC(ip))){
            qDebug()<<ip;
            return ip;
        }
    }
    return QHostAddress();
}

QString IPClassify::getLocalIPString()
{
    QString ipAddr;
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    for(QHostAddress address:AddressList){
            if((address.protocol() == QAbstractSocket::IPv4Protocol)&&(isABC(address))){
                qDebug()<<address;
                return address.toString();
            }
    }
    return ipAddr;
}

void IPClassify::SelfTest()
{
    getLocalIP();
    getLocalIPString();
}
