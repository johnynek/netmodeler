#ifndef starsky__spatialnetwork
#define starsky__spatialnetwork

#include "spatialnode.h"
#include "network.h"
#include "random.h"
#include <queue>
#include <map>

namespace Starsky {

 /**
  * makes a network where the nodes are points are uniformly
  * distributed (in a given n-dimensional hypercube), and
  * then each node is connected to it's k nearest (in the space)
  * nodes.
  */
	
class SpatialNetwork : public Network {

    public:
        SpatialNetwork(int nodes,
		       Random& rand,
		       int connections = 1,
		       int dimension = 2);
    protected:
	int _dim;
	int _cons;
	
};
	
}

#endif
