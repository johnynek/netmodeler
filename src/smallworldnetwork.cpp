#include "smallworldnetwork.h"

using namespace Starsky;
using namespace std;

SmallWorldNetwork::SmallWorldNetwork(int nodes,
		                     double p,
				     Random& rand) : Network(), _rand(rand) {
    _rand.setBoolTrueBias(p);
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
    set<Node*>::iterator i,j;
    i=node_set.begin();
    for(;i != node_set.end(); i++) {
        j = i;
	j++;
	for(; j != node_set.end(); j++) {
            if( _rand.getBool() ) {
                add(Edge(*i,*j));
	    }
	}
    }
}
