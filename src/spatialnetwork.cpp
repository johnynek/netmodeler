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

#include "spatialnetwork.h"

using namespace Starsky;
using namespace std;

SpatialNetwork::SpatialNetwork(int nodes,
		               Random& rand,
			       int cons,
			       int dim) : Network(),
			                        _dim(dim),
						_cons(cons)
{
    while( nodes-- > 0 ) {
        add(new SpatialNode(rand,_dim));
    }

    SpatialNode *start_node = 0, *end_node = 0;
    priority_queue<double> dist_queue;
    multimap<double, SpatialNode*> dist_to_node;
    multimap<double, SpatialNode*>::iterator mm_it;
    double this_dist = 0.0;
    
    NodeIterator ni1 = getNodeIterator();
    while( ni1.moveNext() ) {
	start_node = dynamic_cast<SpatialNode*>( ni1.current() );
	while( dist_queue.empty() == false) {
            dist_queue.pop();	
	}
	int i;
	this_dist = (double)dim;
	end_node = 0;
	dist_to_node.clear();
	for(i = 0; i < cons; i++) {
            dist_queue.push(this_dist); //the largest distance is sqrt(d) which is less than d.
	    dist_to_node.insert( pair<double,SpatialNode*>(this_dist,end_node) );
	}
	
	if( start_node != 0 ) {
          NodeIterator ni2 = getNodeIterator();
	  while( ni2.moveNext() ) {
            end_node = dynamic_cast<SpatialNode*>( ni2.current() );
	    if( (start_node != end_node) && (end_node != 0) ) {
              this_dist = start_node->getDistanceTo( end_node );
	      if( this_dist > 0 ) {
		  if( this_dist < dist_queue.top() ) {
		      //Remove the old node:
		      mm_it = dist_to_node.find( dist_queue.top() );
		      dist_to_node.erase(mm_it);
                      dist_queue.pop();
		      
		      //Add the new one:
		      dist_to_node.insert( pair<double,SpatialNode*>(this_dist,end_node) );
		      dist_queue.push(this_dist);
		  }
	      }
	    }
	  }
	}
	//Now we add the edges to all the nodes:
	for(mm_it = dist_to_node.begin(); mm_it != dist_to_node.end(); mm_it++) {
            add(Edge(start_node, mm_it->second));
	}
    }
}
