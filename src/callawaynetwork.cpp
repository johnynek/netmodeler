#include "callawaynetwork.h"

using namespace Starsky;
using namespace std;

CallawayNetwork::CallawayNetwork(int nodes,
		                 double p,
				 Random& rand) :  _rand(rand), _p(p) {
    incrementTime(nodes);
}

CallawayNetwork::CallawayNetwork(const Network& seed,
		                 double p,
				 Random& rand) : _rand(rand), _p(p)
{
    Network::operator=(seed);
    //We need to fill up the node vector:
    _node_vec.insert(_node_vec.begin(), node_set.begin(), node_set.end() );
}


void CallawayNetwork::incrementTime(int steps) {

  while( steps-- > 0) {
      Node* new_node = new Node();
      if( Network::add( new_node ) )
      {  
          _node_vec.push_back( new_node );
          //Get the current number of nodes that we can connect to
          int nodes = node_set.size();
          //With some probability add an edge:
          if( (nodes > 1) && _rand.getBool(_p) ) {
	    int index_i = 0, index_j = 0;
	    index_i = _rand.getInt(nodes-1);
	    do {
	      index_j = _rand.getInt(nodes-1);
	    } while( index_i == index_j );
	    add(Edge( _node_vec[ index_i ], _node_vec[ index_j ] ));
          }
      }
  }
}
