#include "onnrequest.h"

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
    this->contract = contract;
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
    ls<<"method"<<ecc.publicKeyString<<contract<<method<<arg;
    auto block = ls.join("$");
    auto sig = ecc.Sign(block);
    QString msg = sig + "&" + block;
    return msg;
}
