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

    _rand_gen.setBoolTrueBias(p);

    for(int k = 0; k < nodes; k++) {
        add( new Node() );
    }

    set<Node*>::iterator i,j;
    for(i = node_set.begin();i != node_set.end(); i++) {
      for(j=node_set.begin(); j != node_set.end(); j++) {
        if( _rand_gen.getBool() ) {
          add(DirectedEdge(*i,*j));
	}
      }
    }
}
