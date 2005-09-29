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

#include "magnetmessage.h"

using namespace Starsky;
using namespace std;

MagnetMessage::MagnetMessage(Random& r, double p, int ttl) : _rand(r),
	                                                     _p(p),
							     _ttl(ttl)
{
  forgetVisitedNodes();
}

void MagnetMessage::visit(Node* n, Network& net)
{
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
    while( tv_it != to_visit.end() &&
		    ( (tv_it->first < _ttl) || (_ttl == -1) ) )
    {
        this_distance = tv_it->first + 1;
        //Here are all the nodes at this distance:
        for( a_it = tv_it->second.begin();
	     a_it != tv_it->second.end();
	     a_it++)
	{
            //Now look for a random node to go to:
	    Network* neighbors = net.getNeighbors( *a_it );
	    //Make sure there are some neighbors to go to:
	    if( neighbors->getNodeSize() > 0 ) {
	      Node* vnode = 0;
	      if( _rand.getBool( _p ) ) {
                //select the neighbor with maximum degree
		int max_deg = -1;
		vnode = *n_it;
		NodeIterator ni = neighbors->getNodeIterator();
		while( ni.moveNext() ) {
	          Node* this_node = ni.current();
                  if(max_deg < net.getDegree( this_node )) {
                    max_deg = net.getDegree( this_node );
		    vnode = this_node;
		  }
		}
	      }
	      else {
                //select a random node:
	        rand_node = _rand.getInt( neighbors->getNodeSize() - 1 );
		NodeIterator ni = neighbors->getNodeIterator();
		ni.moveNext();
	        while( rand_node-- > 0 ) { ni.moveNext(); }
		vnode = ni.current();
	      }
	      //Visit the randomly selected node:
	      to_visit[this_distance].insert( vnode );
	      _visited_nodes.insert( vnode );
	      //We must cross exactly one edge to visit the above node:
	      _crossed_edges++;
	    }
	    delete neighbors;
	    neighbors = 0;
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
}
