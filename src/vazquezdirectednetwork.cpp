#include "vazquezdirectednetwork.h"

using namespace Starsky;
using namespace std;

VazquezDirectedNetwork::VazquezDirectedNetwork(int nodes,
		                               double p,
					       Random& r) : DirectedNetwork(),
					                       _p(p),
              						       _rand(r)
{
    _node_vec.reserve(nodes);
    int i;
    for(i = 0; i < nodes; i++) {
        incrementTime();
    }
}

void VazquezDirectedNetwork::incrementTime(int steps) {
	 
  while(steps-- > 0) {
    Node* n = new Node();
    
    if( _node_vec.size() > 0 ) {
      //Holds the nodes we scan for possible connections
      map<int, Network::NodePSet> depth_map;
      map<int, Network::NodePSet>::iterator d_it;
      NodePSet::iterator a_it;
      NodePSet connect_to;
      int this_depth;
   
      _rand.setIntRange( _node_vec.size() - 1);
      Node* first_node = _node_vec[ _rand.getInt() ];
    
      connect_to.insert( first_node );
      _rand.setBoolTrueBias(_p);
      depth_map[0].insert(first_node);
      d_it = depth_map.begin();
      while( d_it != depth_map.end() ) {
	this_depth = d_it->first + 1;
	for(a_it = d_it->second.begin(); a_it != d_it->second.end(); a_it++) {
            if( _rand.getBool() ) {
		//Connect to all the neighbors:
                connect_to.insert( getNeighbors( *a_it).begin(),
				   getNeighbors( *a_it).end() );
		depth_map[this_depth].insert( getNeighbors( *a_it).begin(),
				              getNeighbors( *a_it).end() );
	    }
	}
	//We have processed this entire depth, move on
	depth_map.erase(d_it);
	d_it = depth_map.begin();
      }
      //Now add all the edges:
      for(a_it = connect_to.begin(); a_it != connect_to.end(); a_it++) {
        add( DirectedEdge(n,*a_it) );
      }
    }
    add(n);
    _node_vec.push_back(n);
  }
}
