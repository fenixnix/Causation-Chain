#include "nconsensus.h"

NConsensus::NConsensus()
{

}

void NConsensus::Add(NConsensusMetadata result)
{
    if(votedMember.contains(result.addr)){
        return;
    }
    votedMember.append(result.addr);
    if(!consensus.contains(result.hash)){
        NConsensusUnit block(result.hash, result.getData());
        block.AddMember(result.addr);
        consensus.insert(result.hash,block);
    }else{
        consensus[result.hash].AddMember(result.addr);
    }
}

int NConsensus::maxSize()
{
    int max = 0;
    foreach(NConsensusUnit blk, consensus.values()){
        if(blk.Size()>max){
            max = blk.Size();
            maxSizeHash = blk.datahash;
        }
    }
    return max;
}

QString NConsensus::data()
{
    return consensus[maxSizeHash].data;
}

QByteArray NConsensus::hash()
{
    return maxSizeHash;
}

QString NConsensus::Print()
{
    QString txt = "MaxHash:" + QString(maxSizeHash.toHex()) + "\n";
    QTextStream ts(&txt);
    ts<<"MaxData:"<<data()<<"\n";
    foreach(auto c, consensus){
        ts<<c.Print()<<"\n";
    }
    return txt;
}
