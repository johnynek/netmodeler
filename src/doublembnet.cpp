#include "doublembnet.h"

using namespace Starsky;
using namespace std;

DoubleMBNet::DoubleMBNet(const Network& seed,
		                         Message& aMessage,
					 Random& aRand,
		                         int iterations)
                                         :  MessageBuiltNetwork(seed,
							        aMessage,
								aRand,
								iterations)
{

}

Node* DoubleMBNet::findPartnerFor(Node* start)
{
  Node* target = MessageBuiltNetwork::findPartnerFor(start);
  /**
   * When we find a node, that node also looks for a new neighbor
   */
  if( target != 0 ) {
    Node* target2 = MessageBuiltNetwork::findPartnerFor(target);
    if( target2 != 0 ) {
      add( Edge(target, target2) );
    }
  }
  return target;
}

#if 0
//This needs to be done carefully

const Network::NodePSet MessageBuiltNetwork::getRemoveNodes()
{
  if( _rand.getBool(0.5) ) {
    NodePSet rem;
    NodePSet::const_iterator nit = node_set.begin();
    if( _rand.getBool( 0.0 ) ) {
      //Remove the highest degree node
      Node* max = *nit;
      while( nit != node_set.end() ) {
        if( getDegree(max) < getDegree(*nit) ) {
          max = *nit;
        }
        nit++;
      }
      rem.insert(max);
    }
    else {
      //Return a random node:
      int rn = _rand.getInt( node_set.size() - 1);
      while( rn-- > 0 ) {
        nit++;
      }
    }

    rem.insert(*nit);
    return rem;
  }
  return _empty_nodeset;
}
#endif
