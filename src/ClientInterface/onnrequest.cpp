#include "onnrequest.h"
#include <QStringList>

OnnRequest::OnnRequest()
{

}

void OnnRequest::Init(QString secKey, QString pubKey)
{
    ecc.SetKey(secKey.toLatin1(),pubKey.toLatin1());
    ethAddr = ecc.GetEthAddrStr();
}

void OnnRequest::SetUrlContract(QString url, QString contract)
{
    this->Url = url;
    this->Contract = contract;
    urlContract = url+"/"+contract;
}

QString OnnRequest::Get(QString method, QString arg)
{
    QStringList ls;
    ls<<urlContract;
    ls<<method<<arg<<ethAddr;
    return ls.join("$");
}

QString OnnRequest::Post(QString method, QString arg)
{
    QStringList ls;
    ls<<"method"<<ecc.GetPubKeyStr()<<Contract<<method<<arg;
    auto block = ls.join("$");
    auto sig = ecc.SignHash(block);
    QString msg = sig + "&" + block;
    return msg;
}
