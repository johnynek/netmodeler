#include "percolationmessage.h"

using namespace Starsky;
using namespace std;

PercolationMessage::PercolationMessage(Random& r,
				       double p,
				       int ttl)
: _rand(r), _prob(p), _ttl(ttl) {

    forgetVisitedNodes();	
}

/**
 * We carefully avoid recursion in the below since it could
 * lead to an enormous memory requirement.  The memory of
 * the below function will be at most the size of the network
 */

void PercolationMessage::visit(Node* n, Network& net) {

    Network::ConnectedNodePSet::const_iterator n_it;

    map<int, Network::NodePSet > to_visit;
    map<int, Network::NodePSet >::iterator tv_it;
    Network::NodePSet::iterator a_it;

    to_visit[0].insert(n);
    _visited_nodes.insert(n);
    int this_distance;
    //We loop through at each TTL:
    tv_it = to_visit.begin();
    while( tv_it != to_visit.end() && ( (tv_it->first < _ttl) || (_ttl == -1) ) ) {
        this_distance = tv_it->first + 1;
        //Here are all the nodes at this distance:
        for( a_it = tv_it->second.begin(); a_it != tv_it->second.end(); a_it++) {
            //Now do each randomly:
            for( n_it = net.getNeighbors(*a_it).begin();
	         n_it != net.getNeighbors(*a_it).end();
                 n_it++) {
                if( _rand.getBool(_prob) ) {
		  //We must cross exactly one edge to see if we visit this node
		  _crossed_edges++;
	          if(_visited_nodes.find( *n_it ) == _visited_nodes.end()) {
                    //We have not seen this one yet.
                    to_visit[this_distance].insert( *n_it );
                    _visited_nodes.insert( *n_it );
                  }
		}
            }
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
}
