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

#include "mergenetwork.h"

using namespace Starsky;
using namespace std;

MergeNetwork::MergeNetwork(const Network& seed,
		           Random& r,
			   int cons) : _rand(r), _cons(cons)
{
  Network::operator=(seed);
  //We need to fill up the node vector:
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  while( ni->moveNext() ) {
    _node_vec.push_back( ni->current() );
  }
}

void MergeNetwork::incrementTime(int steps)
{
  while( steps-- > 0) {
    //Get the current number of nodes that we can connect to
    int nodes = getNodeSize();
    int index0 = _rand.getInt(nodes - 1);
    //Don't choose the previous node:
    int index1 = _rand.getInt(nodes - 2);
    if( index1 == index0 ) {
      index1 = (index1 + 1 ) % nodes;
    }
    //Give index0 all index1's neighbors:
    Node* n0 = _node_vec[index0];
    Node* n1 = _node_vec[index1];
    auto_ptr<NodeIterator> ni( getNeighborIterator(n1) );
    while( ni->moveNext() ) 
    {
      Node* this_node = ni->current();
      if( n0 != this_node ) {
        add( Edge(n0, this_node) );
      }
    }
    //Remove all n1's edges:
    EdgeSet e = _node_to_edges[n1];
    EdgeSet::iterator e_it;
    for(e_it = e.begin(); e_it != e.end(); e_it++)
    {
      remove( *e_it );
    }
    //Give n1 some new edges:
    NodePSet new_neighbors;
    while( new_neighbors.size() < _cons )
    {
      Node* neighbor = _node_vec[ _rand.getInt(nodes - 1) ];
      if( neighbor != n1 ) {
        new_neighbors.insert(neighbor);
      }
    }
    //We have the neighbors, now we just make the edges:
    NodePSet::iterator neigh_it;
    for(neigh_it = new_neighbors.begin();
	neigh_it != new_neighbors.end();
	neigh_it++)
    {
      add( Edge(n1, *neigh_it) );
    }
  }

}
