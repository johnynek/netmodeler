#include "inetworkpartitioner.h"

using namespace std;
using namespace Starsky;

void INetworkPartitioner::deletePartition(set<Network*>* part)
{
  set<Network*>::iterator it;
  for(it = part->begin(); it != part->end(); it++)
  {
    //Delete the Network:
    delete *it;
  }
  //Delete the Set:
  delete part;
}

double INetworkPartitioner::modularityOf(set<Network*>& partition,
		                         const Network& orig)
{

  return 0.0;
	
}
