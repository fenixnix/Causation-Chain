#include "nconsensus.h"

NConsensus::NConsensus()
{

}

void NConsensus::Add(NResult result)
{
  if(!consensus.contains(result.dataHash)){
      NConsensusBlock block(result.dataHash, result.data);
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
          maxSizeHash = blk.data;
        }
    }
  return max;
}

QString NConsensus::data()
{
  return consensus[maxSizeHash].data;
}
