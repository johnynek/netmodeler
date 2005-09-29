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

#include "simplerulenet.h"

using namespace Starsky;
using namespace std;


void SimpleRuleNet::incrementTime(int steps)
{
  while(steps-- > 0) {
    Node* new_node = new Node();
    add( new_node );
    //Make the connections:
    int connections = getConnectionCount(new_node);
    while(connections-- > 0) {
      Node* partner = findPartnerFor(new_node);
      //if we can't find a partner, partner == 0)
      if(partner) {
        add( Edge(new_node, partner) );
      }
    }
    //Now find the nodes to remove:
    NodePSet rem = getRemoveNodes();
    //The set of nodes which at the end will be compensated
    NodePSet comped;
    NodePSet::iterator it;
    for( it = rem.begin();
         it != rem.end();
	 it++)
    {
      //First we remove self loops, which should not
      //be there, but are not compensated for:

      remove( Edge(*it, *it) );
	    
      //Find the nodes that will need compensation
      EdgeIterator ei = getEdgeIterator();
      while( ei.moveNext() ) {
	Edge* this_edge = ei.current();
        Node* other = this_edge->getOtherNode( *it );
	if( needsCompensation(other, this_edge) ) {
          comped.insert( other );
	}
      }
      //Now remove the node in question
      remove( *it );
      //Make sure this node is not in our set to be compensated:
      comped.erase( *it );
    }
    //Now compensate the nodes that need it:
    NodePSet::iterator n_it;
    for(n_it = comped.begin();
	  n_it != comped.end();
	  n_it++)
    {
      Node* partner = findPartnerFor( *n_it );
      if( partner ) {
        //cerr << "compensating: (" << *n_it << ", " << partner << ")" << endl;
        if( partner == *n_it) { cerr<<"findPartner found self" <<endl;}
        add( Edge(*n_it, partner) );
      }
    }
    //Now we call the post node addition function:
    postNodeAdd(new_node);
  }
}
