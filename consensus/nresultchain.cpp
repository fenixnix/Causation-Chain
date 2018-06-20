#include "nresultchain.h"

NResultChain::NResultChain()
{

}

void NResultChain::Add(quint64 time, NResult res)
{
  if(!consensus.contains(time)){
      NConsensus con;
      con.Add(res);
      consensus.insert(time,con);
      timeChain.append(time);
      return;
    }else{
      consensus[time].Add(res);
    }
}

int NResultChain::consensusSize(quint64 time)
{
  return consensus[time].maxSize();
}

QString NResultChain::consensusData(quint64 time)
{
  if(consensusSize(time)>0){
      return consensus[time].data();
    }
  return "";
}

int NResultChain::lastSize()
{
  return consensusSize(timeChain.last());
}

QString NResultChain::lastData()
{
  return consensusSize(timeChain.last());
}
