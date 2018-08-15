#include "onnrequest.h"
#include <QStringList>

OnnRequest::OnnRequest()
{

}

void OnnRequest::Init(QString secKey, QString pubKey)
{
    ecc.SetSecKey(secKey);
    ecc.SetPubKey(pubKey);
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
    ls<<method<<arg<<ecc.publicKeyString;
    return ls.join("$");
}

QString OnnRequest::Post(QString method, QString arg)
{
    QStringList ls;
    ls<<"method"<<ecc.publicKeyString<<Contract<<method<<arg;
    auto block = ls.join("$");
    auto sig = ecc.Sign(block);
    QString msg = sig + "&" + block;
    return msg;
}
