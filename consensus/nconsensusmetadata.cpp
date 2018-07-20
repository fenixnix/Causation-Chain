#include "nconsensusmetadata.h"
#include <QJsonDocument>

NConsensusMetadata::NConsensusMetadata()
{

}

NConsensusMetadata::NConsensusMetadata(QString json)
{
    auto jDom = QJsonDocument::fromJson(json.toLatin1());
    auto jObj = jDom.object();
    frame = jObj["TS"].toInt();
    addr = jObj["ID"].toString();
    data = jObj["Data"].toString();
}

NConsensusMetadata::NConsensusMetadata(quint64 ts)
{
    randomTestData(ts);
}

NConsensusMetadata::NConsensusMetadata(quint64 ts, QString id, QString data)
{
    frame = ts;
    addr = id;
    setData(data);
}

QString NConsensusMetadata::getData() const
{
    return data;
}

void NConsensusMetadata::setData(const QString &value)
{
    data = value;
    hash = QCryptographicHash::hash(getData().toLatin1(),QCryptographicHash::Keccak_256);
}

QString NConsensusMetadata::Print()
{
    QString txt;
    QTextStream ts(&txt);
    ts << "T:" <<frame << ",";
    ts << "Addr:"<<addr << ",";
    ts << "Data:"<<data << ",";
    ts << "Hash:"<<QString(hash.toHex());
    return txt;
}

QChar randChar(){
    return QChar(97+qrand()%26);
}

QString randString(int length){
    QString txt;
    QTextStream ts(&txt);
    for(int i = 0;i<length;i++){
        ts<<randChar();
    }
    return txt;
}

void NConsensusMetadata::randomTestData(quint64 ts)
{
    frame = ts;
    addr = randString(4);
    setData(randString(16));
}
