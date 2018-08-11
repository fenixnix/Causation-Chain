#ifndef NECC_H
#define NECC_H

#include <QString>

class NEcc
{
public:
    NEcc();
    void Generate();
    void SetKey(QString secKeyStr, QString pubKeyStr);
    void SetKey(QByteArray secKey, QByteArray pubKey);

    QString SignHash(QString msg);
    QByteArray Sign(QByteArray dat);

    QString GetSecKeyStr();
    QString GetPubKeyStr();
    QString GetCmpKeyStr();

    static bool VerifyCompressKeyHash(QString compressPubKey, QString datStr, QString signStr);
    static bool VerifyHash(QString pubKeyStr, QString datStr, QString signStr);
    static bool Verify(QByteArray pubKey, QByteArray dat, QByteArray sign);

    static QByteArray Compress(QByteArray pubKey);
    static QByteArray DeCompress(QByteArray cmpPubKey);

    static void SelfTest();

private:
    QByteArray secKey;
    QByteArray pubKey;
    QByteArray cmpKey;
};

#endif // NECC_H
