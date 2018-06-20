#include "nconsensusblock.h"

NConsensusBlock::NConsensusBlock(QString hash, QString data)
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
