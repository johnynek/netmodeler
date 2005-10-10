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
}

Network* MagnetMessage::visit(Node* n, Network& net)
{
    map<int, Network::NodePSet > to_visit;
    map<int, Network::NodePSet >::iterator tv_it;
    Network::NodePSet::iterator a_it;
    int this_distance, routes, rand_node;

    to_visit[0].insert(n);
    Network* new_net = net.newNetwork();
    new_net->add(n);
    
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
	    auto_ptr<EdgeIterator> ei( net.getEdgeIterator( *a_it ) );
	    Edge* vedge = 0;
	    Node* vnode = 0;
	    if( _rand.getBool( _p ) ) {
              //select the neighbor with maximum degree
	      int max_deg = -1;
	      while( ei->moveNext() ) {
		Edge* this_edge = ei->current();
                Node* this_node = this_edge->getOtherNode( *a_it );
                if(max_deg < net.getDegree( this_node )) {
                    max_deg = net.getDegree( this_node );
		    vnode = this_node;
		    vedge = this_edge;
		}
	      }
	    }
	    else {
              //select a random edge:
	      int rand_edge = _rand.getInt( net.getDegree( *a_it ) - 1 );
	      ei->moveNext();
	      while( rand_edge-- > 0 ) { ei->moveNext(); }
	      vedge = ei->current();
	      vnode = vedge->getOtherNode( *a_it );
	    }
	    if( vnode != 0 ) {
	      //Visit the randomly selected node:
	      to_visit[this_distance].insert( vnode );
	      new_net->add( vnode );
	      new_net->add( vedge );
	    }
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
  return new_net;
}
