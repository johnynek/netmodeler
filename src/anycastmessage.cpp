#include "anycastmessage.h"

using namespace Starsky;
using namespace std;

AnycastMessage::AnycastMessage(Random& r,
				       int max_routes,
				       int ttl)
: _rand(r), _max(max_routes), _ttl(ttl) {
  forgetVisitedNodes();
}

/**
 * We carefully avoid recursion in the below since it could
 * lead to an enormous memory requirement.  The memory of
 * the below function will be at most the size of the network
 */

void AnycastMessage::visit(Node* n, Network& net) {

    map<int, Network::NodePSet > to_visit;
    map<int, Network::NodePSet >::iterator tv_it;
    Network::ConnectedNodePSet neighbors;
    Network::ConnectedNodePSet::iterator n_it;
    Network::NodePSet::iterator a_it;
    int this_distance, routes, rand_node;

    to_visit[0].insert(n);
    _visited_nodes.insert(n);
    
    //We loop through at each TTL:
    tv_it = to_visit.begin();
    while( tv_it != to_visit.end() && ( (tv_it->first < _ttl) || (_ttl == -1) ) ) {
        this_distance = tv_it->first + 1;
        //Here are all the nodes at this distance:
        for( a_it = tv_it->second.begin(); a_it != tv_it->second.end(); a_it++) {
            //Now look for a random node to go to:
	    neighbors = net.getNeighbors( *a_it );
	    //Make sure there are some neighbors to go to:
	    if( neighbors.size() > 0 ) {
              _rand.setIntRange( neighbors.size() - 1 );
	      for(routes = 0; routes < _max; routes++) {
		//First select a random node:
		rand_node = _rand.getInt();
		n_it = neighbors.begin();
		while( rand_node-- > 0 ) {
                    n_it++;
		}
		//Visit the randomly selected node:
		to_visit[this_distance].insert( *n_it );
		_visited_nodes.insert( *n_it );
		//We must cross one edge to visit the above node:
		_crossed_edges++;
		//Remove this node from consideration for next time:
		neighbors.erase(n_it);
	      }
	    }
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
}
