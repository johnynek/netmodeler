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

vector<Network*>* ComponentPart::partition(const Network& input)
{
    vector<Network*>* out = new vector<Network*>();
    Network* tmp_net = 0;
    
    Network::NodePSet to_check, checked;
    Network::NodePSet::iterator check_it;
    
    //For all nodes
    auto_ptr<NodeIterator> nit( input.getNodeIterator() );
    while( nit->moveNext() ) {
        Node* nodei = nit->current();
	//Check to make sure we have not already identified this component:
	if( checked.find( nodei ) == checked.end() ) {
            to_check.insert( nodei );
            check_it = to_check.begin();
	    //Here we make a new component:
	    tmp_net = input.newNetwork();
            while( check_it != to_check.end() ) {
		tmp_net->add( *check_it );
		auto_ptr<EdgeIterator> ei( input.getEdgeIterator( *check_it ) );
		while( ei->moveNext() ) {
                    tmp_net->add( ei->current() );
	        }
		auto_ptr<NodeIterator> ni( input.getNeighborIterator( *check_it ) );
		while( ni->moveNext() ) {
	            Node* this_node = ni->current();
		    if( checked.find( this_node ) == checked.end() ) {
                      to_check.insert( this_node );
		    }
		    //Else we have already checked this one.
	        }
	        checked.insert( *check_it );
                to_check.erase( check_it );
	        check_it = to_check.begin();
            }
	    checked.insert( nodei );
	    //We have reached the entire component, go to the next.
	    out->push_back( tmp_net );
        }
	else {
	}
    }
    sort(out->begin(), out->end(), networkptr_gt());
    return out;
}
