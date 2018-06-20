#ifndef NEMCC_H
#define NEMCC_H

#include<QString>

class Q_DECL_EXPORT NEmcc
{
public:
    NEmcc();
    void GenerateKeyPair();
    QString Sign(QString data);
    static bool VerifyMsg(QString pubKey, QString msg, QString sign);

    QString privateKeyString;
    QString publicKeyString;
    QString address;
    QByteArray priKey;
    QByteArray pubKey;

    static QString calcAddress(QByteArray pub);
    static QString calcAddress(QString pub);

private:
    static QByteArray RIPEMD_160(QByteArray in);
};

#endif // NEMCC_H
