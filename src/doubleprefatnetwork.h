#ifndef starsky__doubleprefatnetwork
#define starsky__doubleprefatnetwork


#include "network.h"
#include "prefatnetwork.h"
#include "random.h"
#include "edge.h"
#include "degreepreferencefunction.h"
#include "linearpreference.h"

//This is used by one of the methods internally
#include <vector>

namespace Starsky {

 /**
  * constructs a double preferentially attached network in the following way:
  * A node is added and connected to a fixed number of nodes, with
  * Prob(connecting to node with degree k) ~ k.  Then two nodes are selected
  * preferentially and an edge is added between them.
  */
class DoublePrefAtNetwork : public PrefAtNetwork {

    public:
        DoublePrefAtNetwork(const Network& seed,
			    Random& r,
			    int connections_per_step = 1);
	
	DoublePrefAtNetwork(const Network& seed,
			    Random& r,
			    const DegreePreferenceFunction& dp,
			    int connections_per_step = 1);
	
	/**
	 * After each add, we select two nodes at randome and connect them
	 */
	virtual void postNodeAdd(Node* node);
};

}

#endif
