#include "nconsensusblock.h"

NConsensusBlock::NConsensusBlock()
{

}

NConsensusBlock::NConsensusBlock(QByteArray hash, QString data)
{
    datahash = hash;
    this->data = data;
}

void NConsensusBlock::AddMember(QString id)
{
    if(!members.contains(id)){
        members.append(id);
    }
}

int NConsensusBlock::Size()
{
    return members.size();
}

QString NConsensusBlock::Print()
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
