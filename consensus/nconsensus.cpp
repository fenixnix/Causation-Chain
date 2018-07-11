#include "nconsensus.h"

NConsensus::NConsensus()
{

}

void NConsensus::Add(NResult result)
{
  if(!consensus.contains(result.dataHash)){
      NConsensusBlock block(result.dataHash, result.getData());
      block.AddMember(result.id);
      consensus.insert(result.dataHash,block);
    }else{
      consensus[result.dataHash].AddMember(result.id);
    }
}

int NConsensus::maxSize()
{
  int max = 0;
  foreach(NConsensusBlock blk, consensus.values()){
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
