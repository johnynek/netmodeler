#ifndef starsky__callawaynetwork
#define starsky__callawaynetwork

#include "network.h"
#include "incrementable.h"
#include "node.h"
#include "edge.h"
#include "random.h"

#include <vector>


namespace Starsky {

 /**
  * @see cond-mat/0104546.
  * The protocol is simple.  Each time step, add a node, * and with
  * probability p, connect * a pair of nodes.  Note that there will
  * likely be MANY components to this graph
  */
class CallawayNetwork : public Network,
                        public Incrementable {	
    public:
        CallawayNetwork(int steps,
			double p,
			Random& rand);
	CallawayNetwork(const Network& seed,
			double p,
			Random& rand);

	/**
	 * @see Starsky::Incrementable
	 * Takes the next time step
	 */
	virtual void incrementTime(int steps=1);
	
    protected:
	/**
	 * the random number generator which selects the nodes
	 * to connect
	 */
	Random& _rand;
	/**
	 * the probability that a pair of nodes will be connected
	 * at each time step.
	 */
	double _p;
	/**
	 * a vector of all nodes.  This makes selecting a node
	 * at random an O(1) operation rather than O(N)
	 */
	std::vector<Node*> _node_vec;
};
	
}

#endif
