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

#include "smallworldnetwork.h"

using namespace Starsky;
using namespace std;

SmallWorldNetwork::SmallWorldNetwork(int nodes,
		                     double p,
				     Random& rand) : Network(), _p(p), _rand(rand) {
    create(nodes);
}

void SmallWorldNetwork::create(int n) {

    Node* first = new Node();
    Node *tmp, *last = first;
    add(first);
    //We made the first node, now lets make the rest.
    for(int k = 1; k < n; k++) {
        tmp = new Node();
	add(Edge(tmp, last));
	last = tmp;
    }
    //Close the circle
    add(Edge(last, first));

    //This next part is the same as the Random Network:
    NodeIterator ni = getNodeIterator();
    NodeIterator nj;
    while( ni.moveNext() ) {
      Node* nodei = ni.current();
      nj = ni;
      while( nj.moveNext() ) {
        if( _rand.getBool(_p) ) {
          Node* nodej = nj.current();
          add(Edge(nodei,nodej));
	}

      }
    }
}
