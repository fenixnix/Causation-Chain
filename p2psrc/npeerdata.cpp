#include "npeerdata.h"

NPeerData::NPeerData()
{
    pingTime = steady_clock::now();
}

NPeerData::NPeerData(QString id, QIPEndPoint loc, QIPEndPoint nat)
{
    SetData(id,loc,nat);
}

bool NPeerData::SetData(QString data)
{
    auto datas = data.split(',');
    if(data.size()<3){
        return false;
    }
    setId(datas[0]);
    loc.Init(datas[1]);
    nat.Init(datas[2]);
    netInTime = steady_clock::now();
    pingTime = netInTime;
    return true;
}

void NPeerData::SetData(QString id,QIPEndPoint loc, QIPEndPoint nat)
{
    this->id = id;
    this->addr = id.toLatin1().toHex();
    this->loc = loc;
    this->nat = nat;
}

void NPeerData::Ping()
{
    pingTime = steady_clock::now();
    //qDebug()<<"ping"<<id<<pingTime.time_since_epoch().count()/1000000000;
}

void NPeerData::RcvPing()
{
    lastUpdateTime = steady_clock::now();
}

void NPeerData::Pong()
{
    lastUpdateTime = steady_clock::now();
    duration<qint64,std::nano> difT = lastUpdateTime - pingTime;
    ping = difT.count();
    qDebug()<<"ping:"<<id<<(float)ping/1000000.0f<<"ms";
}

bool NPeerData::CheckAlive()
{
    duration<qint64,std::nano> difT = steady_clock::now() - lastUpdateTime;
    int sec = difT.count() /1000000000;
    //qDebug()<<__FUNCTION__<<__LINE__<<sec;
    return sec<=lifeCycle;
}

QString NPeerData::ToString()
{
    QString res = id;
    res += ",";
    res+= loc.ToString();
    res+=",";
    res+= nat.ToString();
    return res;
}

QString NPeerData::getId() const
{
    return id;
}

void NPeerData::setId(const QString &value)
{
    id = value;
    addr = QByteArray::fromHex(value.toLatin1());
}
