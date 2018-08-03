#include "nodeinfo.h"

NodeInfo::NodeInfo()
{
    pingTime = steady_clock::now();
}

NodeInfo::NodeInfo(QString id, QIPEndPoint loc, QIPEndPoint nat)
{
    SetData(id,loc,nat);
}

void NodeInfo::SetData(QString data)
{
    auto datas = data.split(',');
    if(data.size()<3){
        return;
    }
    setId(datas[0]);
    loc.Init(datas[1]);
    nat.Init(datas[2]);
    netInTime = steady_clock::now();
    pingTime = netInTime;
}

void NodeInfo::SetData(QString id,QIPEndPoint loc, QIPEndPoint nat)
{
    this->id = id;
    this->addr = id.toLatin1().toHex();
    this->loc = loc;
    this->nat = nat;
}

void NodeInfo::Ping()
{
    pingTime = steady_clock::now();
    //qDebug()<<"ping"<<id<<pingTime.time_since_epoch().count()/1000000000;
}

void NodeInfo::RcvPing()
{
    lastUpdateTime = steady_clock::now();
}

void NodeInfo::Pong()
{
    lastUpdateTime = steady_clock::now();
    duration<qint64,std::nano> difT = lastUpdateTime - pingTime;
    ping = difT.count();
    qDebug()<<"ping:"<<id<<(float)ping/1000000.0f<<"ms";
}

bool NodeInfo::CheckAlive()
{
    duration<qint64,std::nano> difT = steady_clock::now() - lastUpdateTime;
    int sec = difT.count() /1000000000;
    //qDebug()<<__FUNCTION__<<__LINE__<<sec;
    return sec<=lifeCycle;
}

QString NodeInfo::ToString()
{
    QString res = id;
    res += ",";
    res+= loc.ToString();
    res+=",";
    res+= nat.ToString();
    return res;
}

QString NodeInfo::getId() const
{
    return id;
}

void NodeInfo::setId(const QString &value)
{
    id = value;
    addr = QByteArray::fromHex(value.toLatin1());
}
