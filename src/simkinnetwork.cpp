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
    _rand.setBoolTrueBias(_p);
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
            _rand.setIntRange( nodes - 1 - j);
	    this_idx = _rand.getInt();
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
                if( _rand.getBool() ) {
		    a = node_vec[ *i_it ];
		    b = node_vec[ *j_it ];
                    add( Edge( a, b ) );
		}
	    }
	}
    }
}
