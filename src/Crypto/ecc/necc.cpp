#include "necc.h"
#include "uECC.h"
#include <QCryptographicHash>
#include <QDebug>
#include "utility.h"

#define PRINTDATAARRAY(X) {qDebug()<<QByteArray::fromRawData((char*)X,sizeof(X)).toHex();}
uECC_Curve curve = uECC_secp160r1();
#define KEYSIZE 21
//uECC_Curve curve = uECC_secp256k1();
//#define KEYSIZE 32

NEcc::NEcc()
{

}

QByteArray NEcc::Sign(QByteArray dat)
{
    uint8_t sign[KEYSIZE*2] = {0};
    uECC_sign((uint8_t*)secKey.data(),(uint8_t*)dat.data(),dat.size(),sign,curve);
    return QByteArray((char*)sign,sizeof(sign));
}

QString NEcc::GetSecKeyStr()
{
    return QString(secKey.toHex());
}

QString NEcc::GetPubKeyStr()
{
    return QString(pubKey.toHex());
}

QString NEcc::GetCmpKeyStr()
{
    return QString(cmpKey.toHex());
}

bool NEcc::VerifyCompressKeyHash(QString compressPubKey, QString datStr, QString signStr)
{
    auto pubKey = DeCompress(QByteArray::fromHex(compressPubKey.toLatin1()));
    return VerifyHash(pubKey.toHex(),datStr,signStr);
}

bool NEcc::VerifyHash(QString pubKeyStr, QString datStr, QString signStr)
{
    auto hash = QCryptographicHash::hash(datStr.toLatin1(), QCryptographicHash::Sha1);
    return Verify(QByteArray::fromHex(pubKeyStr.toLatin1()),hash,
                  QByteArray::fromHex(signStr.toLatin1()));
}

bool NEcc::Verify(QByteArray pubKey, QByteArray dat, QByteArray sign)
{
    auto res = uECC_verify((uint8_t*)pubKey.data(),
                          (uint8_t*)dat.data(),dat.size(),
                          (uint8_t*)sign.data(),curve);
    return res == 1;
}

QByteArray NEcc::Compress(QByteArray pubKey)
{
    uint8_t cmpPubKey[KEYSIZE] = {0};
    uECC_compress((uint8_t*)pubKey.data(),cmpPubKey,curve);
    return QByteArray((char*)cmpPubKey,sizeof(cmpPubKey));
}

QByteArray NEcc::DeCompress(QByteArray cmpPubKey)
{
    uint8_t pubKey[KEYSIZE*2] = {0};
    uECC_decompress((uint8_t*)cmpPubKey.data(),pubKey,curve);
    return QByteArray((char*)pubKey,sizeof(pubKey));
}

void NEcc::Generate()
{
    qDebug()<<__FUNCTION__;
    uint8_t secKey[KEYSIZE] = {0};
    uint8_t pubKey[KEYSIZE*2] = {0};
    uECC_make_key(pubKey,secKey,curve);
    this->cmpKey = Compress(this->pubKey);
    SetKey(QByteArray((char*)secKey,KEYSIZE),
           QByteArray((char*)pubKey,KEYSIZE*2));
}

void NEcc::SetKey(QString secKeyStr, QString pubKeyStr)
{
    SetKey(QByteArray::fromHex(secKeyStr.toLatin1()),
           QByteArray::fromHex(pubKeyStr.toLatin1()));
}

void NEcc::SetKey(QByteArray secKey, QByteArray pubKey)
{
    this->secKey = secKey;
    this->pubKey = pubKey;
    qDebug()<<"SecKey:"<<this->secKey.toHex();
    qDebug()<<"PubKey:"<<this->pubKey.toHex();
    this->cmpKey = Compress(this->pubKey);
    qDebug()<<"CmpKey:"<<this->cmpKey.toHex();
}

QString NEcc::SignHash(QString msg)
{
    auto hash = QCryptographicHash::hash(msg.toLatin1(),QCryptographicHash::Sha1);
    return QString(Sign(hash).toHex());
}

void NEcc::SelfTest()
{
    qDebug()<<__FUNCTION__;
    uint8_t secKey[KEYSIZE] = {0};
    uint8_t pubKey[KEYSIZE*2] = {0};
    uECC_make_key(pubKey,secKey,curve);
    PRINTDATAARRAY(secKey);
    PRINTDATAARRAY(pubKey);

    uint8_t msg[32] = {1};
    uint8_t sign[KEYSIZE*2] = {0};
    uECC_sign(secKey,msg,sizeof(msg),sign,curve);
    PRINTDATAARRAY(sign);

    qDebug()<<uECC_verify(pubKey,msg,sizeof(msg),sign,curve);

    uint8_t cmpPubKey[KEYSIZE] = {0};
    uECC_compress(pubKey,cmpPubKey,curve);
    PRINTDATAARRAY(cmpPubKey);
    uECC_decompress(cmpPubKey,pubKey,curve);
    PRINTDATAARRAY(pubKey);
    qDebug()<<"NEcc Test:";

    NEcc ecc;
    StopWatch sw;
    ecc.Generate();
    qWarning()<<sw.Count()<<"ms";

    sw.Start();
    auto deCmpKey = DeCompress(ecc.cmpKey);
    qWarning()<<sw.Count()<<"ms";
    qDebug()<<"DeCompress Key"<<deCmpKey.toHex();

    QString message = "Hello world";
    QByteArray dat = message.toLatin1();

    sw.Start();
    auto eccSign = ecc.Sign(dat);
    for(int i = 0;i<9;i++){
        ecc.Sign(dat);
    }
    qWarning()<<sw.Count()<<"ms";
    qDebug()<<"Sign 10times:"<<eccSign.toHex();

    sw.Start();
    auto res = ecc.Verify(ecc.pubKey,dat,eccSign);
    for(int i = 0;i<9;i++){
        ecc.Verify(ecc.pubKey,dat,eccSign);
    }
    qWarning()<<sw.Count()<<"ms";
    qDebug()<<"Verify 10times:"<<res;
}
