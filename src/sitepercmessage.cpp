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

#include "sitepercmessage.h"

using namespace Starsky;
using namespace std;

SitePercMessage::SitePercMessage(Random& r,
				       double p,
				       int ttl)
: _rand(r), _prob(p), _ttl(ttl) {

    forgetVisitedNodes();	
}


void SitePercMessage::visit(Node* n, Network& net)
{
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
	  if( _rand.getBool(_prob) ) {
            NodeIterator* ni = net.getNeighborIterator( *a_it );
	    while( ni->moveNext() ) {
              Node* this_neigh = ni->current();
                if( _visited_nodes.find( this_neigh ) == _visited_nodes.end() ) {
                    //We have not seen this one yet.
                    to_visit[this_distance].insert( this_neigh );
                    _visited_nodes.insert( this_neigh );
                }
            }
	    delete ni;
	    ni = 0;
	    //We must cross all neighbor edges except the one we came in on
	    _crossed_edges += net.getDegree(*a_it) - 1;
	  }//end of if-loop for _prob
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
}

