#ifndef starsky__walkandpercmessage
#define starsky__walkandpercmessage

#include "node.h"
#include "network.h"
#include "random.h"
#include "percolationmessage.h"
#include "anycastmessage.h"
#include <set>
#include <map>

namespace Starsky {

 /**
  * Message which is both a random walker and a percolation message.
  * First it uses the AnycastMessage to make a random walk of length ttl.
  * Then, for every node visited by the AnycastMessage, a PercolationMessage with
  * ttl = -1 (infinite) visits each node.
  * @see Starsky::PercolationMessage
  * @see Starsky::AnyCastMessage
  */
class WalkAndPercMessage : public PercolationMessage {

    public:
	/**
	 * @param p probability that each edge gets the message
	 * @param walkttl the total depth from start node the walk will go
	 * @param percttl the depth the percolation will go
	 */
        WalkAndPercMessage(Random& r, double p, int walkttl, int percttl=-1);
	void visit(Node*, Network& aNet);
    protected:
	Random& _rand;
	AnycastMessage _ac_mes;
	double _prob;
};

}

#endif
