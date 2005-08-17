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

#include "componentpart.h"

using namespace std;
using namespace Starsky;

set<Network*>* ComponentPart::partition(const Network& input)
{
    set<Network*>* out = new set<Network*>();
    Network* tmp_net = 0;
    
    Network::NodePSet to_check, checked;
    Network::NodePSet::const_iterator i;
    Network::NodePSet::iterator check_it;
    
    Network::ConnectedNodePSet::const_iterator n_it;
    
    Network::EdgeSet tmp_edges;
    Network::EdgeSet::const_iterator e_it;
    
    //For all nodes
    const Network::NodePSet& node_set = input.getNodes();
    for(i = node_set.begin(); i != node_set.end(); i++) {
	//Check to make sure we have not already identified this component:
	if( checked.find( *i ) == checked.end() ) {
            to_check.insert( *i );
            check_it = to_check.begin();
	    //Here we make a new component:
	    tmp_net = new Network();
            while( check_it != to_check.end() ) {
		tmp_net->add( *check_it );
                tmp_edges = input.getEdges( *check_it );
	        for(e_it = tmp_edges.begin(); e_it != tmp_edges.end(); e_it++) {
                    tmp_net->add( *e_it );
	        }
	        for(n_it = input.getNeighbors( *check_it ).begin();
		    n_it != input.getNeighbors( *check_it ).end();
		    n_it++) {
		    if( checked.find( *n_it ) == checked.end() ) {
                      to_check.insert( *n_it );
		    }
		    //Else we have already checked this one.
	        }
	        checked.insert( *check_it );
                to_check.erase( check_it );
	        check_it = to_check.begin();
            }
	    checked.insert( *i );
	    //We have reached the entire component, go to the next.
	    out->insert( tmp_net );
        }
	else {
	}
    }
    return out;
}
