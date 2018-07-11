#include "ncause.h"
#include <QJsonDocument>

NCause::NCause()
{

}

NCause::NCause(QString json)
{
    auto jDom = QJsonDocument::fromJson(json.toLatin1());
    auto jObj = jDom.object();
    timeStamp = jObj["TS"].toInt();
    id = jObj["ID"].toString();
    pubKey = jObj["PubKey"].toString();
    data = jObj["Data"].toString();
}

NCause::NCause(quint64 ts)
{
    randomTestData(ts);
}

NCause::NCause(quint64 ts, QString id, QString data)
{
    timeStamp = ts;
    this->id = id;
    this->data = data;
}

QString NCause::getData() const
{
    return data;
}

void NCause::setData(const QString &value)
{
    data = value;
}

QString NCause::Print()
{
    QString txt;
    QTextStream ts(&txt);
    ts << "T:" <<timeStamp << ",";
    ts << "Addr:"<<id << ",";
    ts << "PubKey:" <<pubKey << ",";
    ts << "Data:"<<data;
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

void NCause::randomTestData(quint64 ts)
{
    timeStamp = ts;
    id = randString(4);
    data = randString(16);
    pubKey = randString(32);
}
