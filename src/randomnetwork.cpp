#include "randomnetwork.h"

using namespace Starsky;
using namespace std;

RandomNetwork::RandomNetwork(int nodes,
                             double p,
			     Random& rand) : Network(),
                                              _rand_gen(rand) {
  create(nodes,p);
}
		
void RandomNetwork::create(int nodes, double p) {

    _rand_gen.setBoolTrueBias(p);

    for(int k = 0; k < nodes; k++) {
        add( new Node() );
    }

    set<Node*>::iterator i,j;
    i = node_set.begin();
    for(;i != node_set.end(); i++) {
      j = i;
      j++;
      for(; j != node_set.end(); j++) {
        if( _rand_gen.getBool() ) {
          add( Edge(*i,*j) );
	}
      }
    }
}
