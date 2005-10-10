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

}


Network* SitePercMessage::visit(Node* n, Network& net)
{
    map<int, Network::NodePSet > to_visit;
    map<int, Network::NodePSet >::iterator tv_it;
    Network::NodePSet::iterator a_it;

    to_visit[0].insert(n);
    Network* new_net = net.newNetwork();
    new_net->add(n);
    int this_distance;
    //We loop through at each TTL:
    tv_it = to_visit.begin();
    while( tv_it != to_visit.end() && ( (tv_it->first < _ttl) || (_ttl == -1) ) ) {

        this_distance = tv_it->first + 1;
        //Here are all the nodes at this distance:
        for( a_it = tv_it->second.begin(); a_it != tv_it->second.end(); a_it++) {
	  if( _rand.getBool(_prob) ) {
            auto_ptr<EdgeIterator> ei( net.getEdgeIterator( *a_it ) );
	    while( ei->moveNext() ) {
	      Edge* this_edge = ei->current();
              Node* this_neigh = this_edge->getOtherNode( *a_it );
	      if( !new_net->has( this_neigh ) ) {
                    //We have not seen this one yet.
                    to_visit[this_distance].insert( this_neigh );
                    new_net->add( this_neigh );
              }
	      new_net->add( this_edge );  
            }
	  }//end of if-loop for _prob
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
    return new_net;
}

