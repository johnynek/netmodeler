#ifndef starsky__degreelawrandomnetwork
#define starsky__degreelawrandomnetwork

#include "network.h"
#include "node.h"
#include "degreeprobabilityfunction.h"
#include "random.h"

#include <vector>
#include <set>
#include <algorithm>

namespace Starsky {

  /**
   * a random network with a degree distribution given.
   */

class DegreeLawRandomNetwork : public Network {

    public:
	/**
	 * @param nodes the size of the network
	 * @param dpf the probability function you want for the degree dist
	 * @param rand Random number generator used to randomly form edges
	 * @param indep If true, each node is assigned a degree independently
	 *   according to the dpf.  If false, then the dpf gives the FRACTION
	 *   of nodes with a given degree.  This means that if one selects a 
	 *   node at random, the probability distribution on degrees will be dpf
	 */
        DegreeLawRandomNetwork(int nodes,
			       DegreeProbabilityFunction& dpf,
			       Random& rand,
			       bool indep=false);
    protected:
        DegreeProbabilityFunction& _dpf;
	Random& _rand;
	
};
	
}

#endif
