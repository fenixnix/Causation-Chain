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

    void SetSecKey(QString key);
    void SetPubKey(QString key);

    QString privateKeyString;
    QString publicKeyString;
    QString address;
    QString ethAddr;
    QByteArray priKey;
    QByteArray pubKey;

    static bool CheckEthAddr(QString addr);
    static QString CalcAddr(QByteArray pub);
    static QString CalcAddr(QString pub);
    static QString CalcETHAddr(QString pub);

private:
    static QByteArray RIPEMD_160(QByteArray in);
};

#endif // NEMCC_H
