#include "nodeinfo.h"

NodeInfo::NodeInfo()
{

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
    netInTime = QDateTime::currentDateTime();
    lastHeatbeatTime = netInTime;
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
    lastHeatbeatTime = QDateTime::currentDateTime();
    //qDebug()<<__FUNCTION__<<1<<lastHeatbeatTime.toString();
}

bool NodeInfo::CheckAlive()
{
    auto t = lastHeatbeatTime.secsTo(QDateTime::currentDateTime());
    //qDebug()<<__FUNCTION__<<t<<lastHeatbeatTime.toString();
    return t<=lifeCycle;
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
