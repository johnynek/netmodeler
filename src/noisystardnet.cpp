#include "noisystardnet.h"

using namespace Starsky;
using namespace std;

NoisyStarDNet::NoisyStarDNet(double p, Random& ran,
		             double del, double rest) :
                                                      _p(p),
						      _rand(ran),
						      _del(del),
						      _rest(rest)
{
    
}

Node* NoisyStarDNet::findPartnerFor(Node* s)
{
  Node* tmp = 0;
  //This network only has one node
  if( has(s) && (node_set.size() == 1) ) { return tmp; }
  //Get a random node in the network:
  NodePSet::const_iterator nit = node_set.begin();
  int r_n = _rand.getInt(node_set.size() - 1);
  if(has(s)) { r_n--; }
  while( r_n-- > 0 ) { nit++; }
  if( *nit == s ) { nit++; }
  tmp = *nit;
  //Now tmp has a random node in the network.
  if( _rand.getBool(_p) ) {
    return tmp;
  }
  //else:
  //we connect to tmp's neighbor of greatest degree:
  ConnectedNodePSet::const_iterator c_it;
  int max_degree = getDegree(tmp);
  Node* max_node = tmp;
  //Hack to randomly connect to a neighbor:
  int n = _rand.getInt( getNeighbors(tmp).size(), 1 );
  for(c_it = getNeighbors(tmp).begin();
      c_it != getNeighbors(tmp).end();
      c_it++)
  {
#if 1
      //Connect to the maximum degree that is not the start node: 
      if( (s != *c_it) && (max_degree < getDegree(*c_it)) )
      {
          max_degree = getDegree(*c_it);
	  max_node = *c_it;
      }
#else
      //The below was a hack to select an edge randomly 
      if(n-- == 0) { max_node = *c_it; break; }
#endif
  }
  return max_node;
}

const Network::NodePSet NoisyStarDNet::getRemoveNodes()
{
 if( _rand.getBool( _del ) ) {
  //Select a random node:
  int r = _rand.getInt( node_set.size() - 1 );
  NodePSet::const_iterator it = node_set.begin();
  while( r-- > 0 ) {
    it++;
  }
  NodePSet ret;
  ret.insert( *it );
  return ret;
 }
 return Network::_empty_nodeset;
}
