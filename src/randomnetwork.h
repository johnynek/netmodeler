#ifndef starsky__randomnetwork_h
#define starsky__randomnetwork_h

#include "network.h"
#include "random.h"


namespace Starsky {

 /**
  * Produces random networks where each edge exists with probability p
  */
class RandomNetwork : public Network {

    public:
        RandomNetwork(int nodes,
	              double p,
		      Random& rand);

    protected:
        Random& _rand_gen;
	
        void create(int n, double p); 

};

}

#endif
