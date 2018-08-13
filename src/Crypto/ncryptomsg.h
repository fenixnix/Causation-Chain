#ifndef NCRYPTOMSG_H
#define NCRYPTOMSG_H

#include <QString>
#include "nemcc.h"

class NCryptoMsg
{
public:
    NCryptoMsg();
    void GenerateKey();
    void SetKey(QString secKey, QString pubKey);
    void LoadConfigFile(QString fileName);
    QString getAddr();
    QString getPubKeyStr();
    QByteArray getSecKey();
    QByteArray getPubKey();

    QString encode(QString msg);
    QStringList decode(QString msg, QMap<QString, QString> keyMap);//timeStamp, addr, msg

    QString encodeWithPubKey(QString msg);
    QStringList decodeWithPubKey(QString msg);//timeStamp, addr, msg

private:
    NEmcc ecc;
    QString Dsa(QString msg);
    QString DsaWithPubKey(QString msg);
    QString GetMsgTSString(QString msg);
};

#endif // NCRYPTOMSG_H
