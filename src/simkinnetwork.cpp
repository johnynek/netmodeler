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

#include "simkinnetwork.h"

using namespace Starsky;
using namespace std;

SimkinNetwork::SimkinNetwork(int nodes,
		                 int group_size,
		                 double p,
				 Random& rand,
				 int repeat) : Network(),
				                  _p(p),
						  _rand(rand) {
    create(nodes,group_size,repeat);
}

void SimkinNetwork::create(int nodes, int group_size, int repeat) {
    Node* new_node;
    vector<Node*> node_vec;
    node_vec.reserve(nodes);
    //Make all the nodes:
    for(int i = 0; i < nodes; i++) {
        new_node = new Node();
	node_vec.push_back(new_node);
	add( new_node );
    }
    //Add all the edges:
    if( repeat == -1 ) {
        repeat = nodes / group_size;
    }
    set<int> selected_idx;
    set<int>::const_iterator i_it, j_it;
    int this_idx;
    Node *a, *b;
    for(int i = 0; i < repeat; i++) {
	selected_idx.clear();
	//Random selection without replacement:
        for(int j = 0; j < group_size; j++) {
	    this_idx = _rand.getInt( nodes - 1 - j );
	    if( selected_idx.count(this_idx) == 0 ) {
                selected_idx.insert( this_idx );
	    }
	    else { //We take the one after, etc..
                while( selected_idx.count( this_idx ) == 0) {
                    this_idx++;
		}
		selected_idx.insert(this_idx);
	    }
	}
	for( i_it = selected_idx.begin(); i_it != selected_idx.end(); i_it++) {
            j_it = i_it;
	    j_it++;
	    for(; j_it != selected_idx.end(); j_it++) {
                if( _rand.getBool(_p) ) {
		    a = node_vec[ *i_it ];
		    b = node_vec[ *j_it ];
                    add( Edge( a, b ) );
		}
	    }
	}
    }
}
