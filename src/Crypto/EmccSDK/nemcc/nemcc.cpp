#include "nemcc.h"
#include "emcclib.h"
#include <QCryptographicHash>
#include <QDebug>

NEmcc::NEmcc()
{

}

QString Hash256(QString data){
    auto hashData = QCryptographicHash::hash(data.toLocal8Bit(),QCryptographicHash::Sha256);
    return hashData.toHex();
}

void NEmcc::GenerateKeyPair()
{
    char tmpData[192];
    int length;
    GetKeyPair(tmpData,length);
    QByteArray br(tmpData,length);
    privateKeyString = QString(br.left(64));
    publicKeyString = QString(br.right(128));
    priKey.clear();
    pubKey.clear();
    priKey = QByteArray::fromHex(privateKeyString.toLatin1());
    pubKey = QByteArray::fromHex(publicKeyString.toLatin1());
    address = CalcAddr(pubKey);
    ethAddr = CalcETHAddr(publicKeyString);
    //qDebug()<<"Address:"<<address<<address.size();
}

QString NEmcc::Sign(QString data)
{
    auto hashData = Hash256(data);
    char tmpData[128];
    int length;
    GetSignData(privateKeyString.toLocal8Bit().data(),hashData.toLocal8Bit().data(),tmpData,length);
    QByteArray br(tmpData,length);
    return QString(br);
}

bool NEmcc::VerifyMsg(QString pubKey, QString msg, QString sign)
{
    auto hashData = Hash256(msg);
    //qDebug()<<"Hash Data:"<<QString(hashData);
    return Verify(pubKey.toLocal8Bit().data(),
                  hashData.toLocal8Bit().data(),
                  sign.toLocal8Bit().data());
}

void NEmcc::SetSecKey(QString key)
{
    privateKeyString = key;
    priKey = QByteArray::fromHex(privateKeyString.toLatin1());
}

void NEmcc::SetPubKey(QString key)
{
    publicKeyString = key;
    pubKey = QByteArray::fromHex(publicKeyString.toLatin1());
    address = CalcAddr(key);
    ethAddr = CalcETHAddr(publicKeyString);
}

bool NEmcc::CheckEthAddr(QString addr)
{
    QRegExp reg("[0-9a-fA-F]{40}");
    return reg.exactMatch(addr);
}

QString NEmcc::CalcAddr(QByteArray pub)
{
    //qDebug()<<pub.size()<<pub;
    auto pubhash = QCryptographicHash::hash(pub,QCryptographicHash::Keccak_256);
    //qDebug()<<pubhash.size()<<pubhash;
    auto ripeMD = RIPEMD_160(pubhash);
    //qDebug()<<ripeMD.size();
    QByteArray WithNetID;
    WithNetID.append((char)0);
    WithNetID.append(ripeMD);
    auto haha = QCryptographicHash::hash(QCryptographicHash::hash(WithNetID,QCryptographicHash::Keccak_256),QCryptographicHash::Keccak_256);
    auto first4 = haha.left(4);
    QByteArray finalData;
    finalData.append(WithNetID);
    finalData.append(first4);
    return QString(finalData.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
}

QString NEmcc::CalcAddr(QString pub)
{
    return CalcAddr(QByteArray::fromHex(pub.toLatin1()));
}

QString NEmcc::CalcETHAddr(QString pub)
{
    return QString(QCryptographicHash::hash(pub.toLatin1(),QCryptographicHash::Keccak_256).toHex().left(40));
}

QByteArray NEmcc::RIPEMD_160(QByteArray in)
{
    return QCryptographicHash::hash(in,QCryptographicHash::Sha1);
}
