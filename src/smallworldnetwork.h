#ifndef starsky__smallworldnetwork
#define starsky__smallworldnetwork


#include "network.h"
#include "random.h"
#include "node.h"
#include <set>

namespace Starsky {

    /**
     * Watts Strogatz model for a 1-D small world.  Nodes are arranged
     * in a ring, and with probability p each pair is also connected.
     */
class SmallWorldNetwork : public Network {

    public:
        SmallWorldNetwork(int nodes, double p, Random& rand);
	
    protected:
	void create(int n);
	Random& _rand;
	
};
	
}

#endif
