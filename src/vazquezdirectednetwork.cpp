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
   
      Node* first_node = _node_vec[ _rand.getInt( _node_vec.size() - 1 ) ];
    
      connect_to.insert( first_node );
      depth_map[0].insert(first_node);
      d_it = depth_map.begin();
      while( d_it != depth_map.end() ) {
	this_depth = d_it->first + 1;
	for(a_it = d_it->second.begin(); a_it != d_it->second.end(); a_it++) {
            if( _rand.getBool(_p) ) {
		//Connect to all the neighbors:
		NodeIterator* ni = getNeighborIterator( *a_it );
		while( ni->moveNext() ) {
	          Node* this_neighbor = ni->current();
                  connect_to.insert( this_neighbor );
		  depth_map[this_depth].insert( this_neighbor );
		}
		delete ni;
		ni = 0;
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
    Network::add(n);
    _node_vec.push_back(n);
  }
}
