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

void NodeInfo::HeartBeat()
{
    //qDebug()<<__FUNCTION__<<lastHeatbeatTime.toString();
    pingTime = steady_clock::now();
    //qDebug()<<__FUNCTION__<<1<<lastHeatbeatTime.toString();
}

void NodeInfo::Ping()
{
    pingTime = steady_clock::now();
    //qDebug()<<"ping"<<id<<pingTime;
}

void NodeInfo::Pong()
{
    ping = steady_clock::now().time_since_epoch().count() - pingTime.time_since_epoch().count();
    qDebug()<<"ping:"<<id<<(float)ping/1000000.0f<<"ms";
}

bool NodeInfo::CheckAlive()
{
    auto t = steady_clock::now().time_since_epoch().count()
            - pingTime.time_since_epoch().count();
    int sec = t/1000000000;
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
