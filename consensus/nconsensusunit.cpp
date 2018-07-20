#include "nconsensusunit.h"

NConsensusUnit::NConsensusUnit()
{

}

NConsensusUnit::NConsensusUnit(QByteArray hash, QString data)
{
    datahash = hash;
    this->data = data;
}

void NConsensusUnit::AddMember(QString id)
{
    if(!members.contains(id)){
        members.append(id);
    }
}

int NConsensusUnit::Size()
{
    return members.size();
}

QString NConsensusUnit::Print()
{
    QString txt;
    QTextStream ts(&txt);
    ts<<"+Group+*"<<Size()<<":";
    foreach(auto m, members){
        ts<<m<<",";
    }
    ts<<"Data&Hash:"<<data<<"*"<<datahash.toHex()<<"\n";
    return txt;
}
