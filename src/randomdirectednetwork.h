#ifndef starsky__randomdirectednetwork_h
#define starsky__randomdirectednetwork_h

#include "directednetwork.h"
#include "random.h"
#include "directededge.h"

 /**
  * Produces random networks where each edge exists with probability p
  */

namespace Starsky {

class RandomDirectedNetwork : public DirectedNetwork {

    public:
        RandomDirectedNetwork(int nodes,
	              double p,
		      Random& rand);

    protected:
        Random& _rand_gen;
	
        void create(int n, double p); 

};

}

#endif
