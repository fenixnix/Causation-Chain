#include "nresult.h"

NResult::NResult()
{

}

NResult::NResult(QString json)
{
    NCause::NCause(json);
    dataHash = QCryptographicHash::hash(getData().toLatin1(),QCryptographicHash::Keccak_256);
}

NResult::NResult(quint64 ts, QString id, QString data)
{
    this->timeStamp = ts;
    this->id = id;
    setData(data);
}

NResult::NResult(quint64 ts, QString id, QString pubKey, QString data)
{
    this->timeStamp = ts;
    this->id = id;
    this->pubKey = pubKey;
    setData(data);
}

NResult::NResult(quint64 ts)
{
    randomTestData(ts);
    setData(getData());
}

void NResult::setData(const QString &value)
{
    NCause::setData(value);
    dataHash = QCryptographicHash::hash(getData().toLatin1(),QCryptographicHash::Keccak_256);
}

QString NResult::Print()
{
    QString txt = NCause::Print();
    QTextStream ts(&txt);
    ts <<",Hash:"<<QString(dataHash.toHex());
    return txt;
}
