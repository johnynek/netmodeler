/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

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
	    Network* star = net.getEdges( *a_it );
	    //Make sure there are some neighbors to go to:
	    if( star->getEdgeSize() > 0 ) {
	      for(routes = 0; routes < _max; routes++) {
		//First select a random node:
		rand_node = _rand.getInt( star->getEdgeSize() - 1);
		EdgeIterator ei = star->getEdgeIterator();
		ei.moveNext();
		while( rand_node-- > 0 ) {
                    ei.moveNext();
		}
		Node* other = ei.current()->getOtherNode( *a_it );
		//Visit the randomly selected node:
		to_visit[this_distance].insert( other );
		_visited_nodes.insert( other );
		//We must cross one edge to visit the above node:
		_crossed_edges++;
		//Remove this node from consideration for next time:
		star->remove( ei.current() );
	      }
	    }
	    delete star;
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
}
