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

#include "callawaynetwork.h"

using namespace Starsky;
using namespace std;

CallawayNetwork::CallawayNetwork(int nodes,
		                 double p,
				 Random& rand) :  _rand(rand), _p(p) {
    incrementTime(nodes);
}

CallawayNetwork::CallawayNetwork(const Network& seed,
		                 double p,
				 Random& rand) : _rand(rand), _p(p)
{
    Network::operator=(seed);
    //We need to fill up the node vector:
    NodeIterator ni = getNodeIterator();
    while( ni.moveNext() ) {
      _node_vec.push_back( ni.current() );
    }
}


void CallawayNetwork::incrementTime(int steps) {

  while( steps-- > 0) {
      Node* new_node = new Node();
      if( Network::add( new_node ) )
      {  
          _node_vec.push_back( new_node );
          //Get the current number of nodes that we can connect to
          int nodes = getNodeSize();
          //With some probability add an edge:
          if( (nodes > 1) && _rand.getBool(_p) ) {
	    int index_i = 0, index_j = 0;
	    index_i = _rand.getInt(nodes-1);
	    do {
	      index_j = _rand.getInt(nodes-1);
	    } while( index_i == index_j );
	    add(Edge( _node_vec[ index_i ], _node_vec[ index_j ] ));
          }
      }
  }
}
