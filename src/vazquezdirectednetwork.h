#ifndef starsky__vazquezdirectednetwork
#define starsky__vazquezdirectednetwork

#include "random.h"
#include "node.h"
#include "directededge.h"
#include "directednetwork.h"
#include "incrementable.h"

#include <vector>
#include <set>
#include <map>

namespace Starsky {

 /**
  * @see cond-mat/0105031
  * Model for citation networks and possibly software dependency networks.
  * The model works in the following way: As a node is added, it randomly
  * selects a node to connect to.  If a link is added to a node, with
  * probability p, a link is also added to all its neighbors.
  */
class VazquezDirectedNetwork : public DirectedNetwork,
                               public Incrementable {

    public:
        VazquezDirectedNetwork(int nodes, double p, Random& r);

    protected:
	/**
	 * add a node to the network, and do the connections.
	 */
	virtual void incrementTime(int steps = 1);
	Random& _rand;
	double _p;
	std::vector<Node*> _node_vec;
	
};
	
}

#endif
