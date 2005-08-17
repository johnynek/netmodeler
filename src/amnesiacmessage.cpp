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

#include "amnesiacmessage.h"

using namespace Starsky;
using namespace std;

AmnesiacMessage::AmnesiacMessage(Random& r,
				       int ttl)
: _rand(r), _ttl(ttl) {
  forgetVisitedNodes();
}

/**
 * We carefully avoid recursion in the below since it could
 * lead to an enormous memory requirement.  The memory of
 * the below function will be at most the size of the network
 */

void AmnesiacMessage::visit(Node* n, Network& _my_net) {
	
   Network::ConnectedNodePSet neighbors;
   set< pair<Node*,int> > to_visit;
   int new_ttl = _ttl, n_count = 0, rand_neighbor;

   Node* last_node = 0;
   
   to_visit.insert( pair<Node*,int>(n,_ttl) );
   set< pair<Node*,int> >::iterator j = to_visit.begin();
   Network::ConnectedNodePSet::const_iterator i;
   while( j != to_visit.end() ) {
       if( j->second > 0 ) {
           //We don't forward messages with 0 TTL and we never forward TTL == -1
           neighbors = _my_net.getNeighbors( j->first );
           n_count = neighbors.size();
	   //See if we have any neighbors to go to:
	   if( n_count > 0 ) {
	     _rand.setIntRange(n_count - 1);
	     //Get a random edge
	     rand_neighbor = _rand.getInt();
	     i = neighbors.begin();
	     for(int k = 0; k < rand_neighbor; k++) {
	         i++;
	     }
	     //We know the TTL was > 0, so new_ttl is a legitimate number
	     new_ttl = j->second - 1; 
	     to_visit.insert( pair<Node*,int>(*i,new_ttl) ); 
	     //We must cross one edge to visit the above node:
	     _crossed_edges++;
	   }
	   last_node = j->first;
       }
       //This will invalidate the j iterator:
       to_visit.erase( j );
       //So we reset it:
       j = to_visit.begin();
   }
   _visited_nodes.insert( last_node );
}
