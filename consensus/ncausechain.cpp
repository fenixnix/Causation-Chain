#include "ncausechain.h"

NCauseChain::NCauseChain()
{

}

void NCauseChain::Add(quint64 time, NCause cause)
{
  causes.insert(time, cause);
}
