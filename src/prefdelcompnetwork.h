#ifndef starsky__prefdelcompnetwork
#define starsky__prefdelcompnetwork

#include "network.h"
#include "prefatnetwork.h"
#include "random.h"

namespace Starsky {

/**
 * This is a normal preferential attachment network, except
 * at each time step after a node is added, with probability p, a node is deleted.
 * Each node which looses a link, with probability q, will make a
 * preferential connection to restore the link.
 *
 * The name comes from Preferential Deletion Compenstation.
 */

class PrefDelCompNetwork : public PrefAtNetwork {

    public:
	PrefDelCompNetwork(const Network& seed,
			   Random& r,
			   double p=1.0,
			   int connections = 1,
			   double q=1.0);
	PrefDelCompNetwork(const Network& seed,
			   Random& r,
			   const DegreePreferenceFunction& dp,
			   double p=1.0,
			   int connections = 1,
			   double q=1.0);

	//These are implemented from SimpleRuleNet
	/**
	 * This will be zero or one node selected randomly
	 */
	virtual const NodePSet getRemoveNodes();
	/**
	 * @returns true with probability _restore_q
	 */
	virtual bool needsCompensation(Node* n, Edge* e);
    protected:
	/**
	 * the probability that we select a node for deletion
	 */
	double _del_p;
	/**
	 * the probability an edge is restored
	 */
	double _restore_q;
	
};
	
}

#endif
