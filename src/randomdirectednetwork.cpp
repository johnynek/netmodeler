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

#include "randomdirectednetwork.h"

using namespace Starsky;
using namespace std;

RandomDirectedNetwork::RandomDirectedNetwork(int nodes,
                             double p,
			     Random& rand) : 
                                              _rand_gen(rand) {
  create(nodes,p);
}
		
void RandomDirectedNetwork::create(int nodes, double p) {

    for(int k = 0; k < nodes; k++) {
        add( new Node() );
    }

    set<Node*>::iterator i,j;
    NodeIterator ni = getNodeIterator();
    NodeIterator nj = getNodeIterator();
    while( ni.moveNext() ) {
      while( nj.moveNext() ) {
        if( _rand_gen.getBool(p) ) {
          Node* nodei = ni.current();
	  Node* nodej = nj.current();
          add(DirectedEdge(nodei,nodej));
	}
      }
    }
}
