#ifndef starsky__walkandsitepercmessage
#define starsky__walkandsitepercmessage

#include "node.h"
#include "network.h"
#include "random.h"
#include "sitepercmessage.h"
#include "anycastmessage.h"
#include <set>
#include <map>

namespace Starsky {

 /**
  * Message which is both a random walker and a site percolation message.
  * First it uses the AnycastMessage to make a random walk of length ttl.
  * Then, for every node visited by the AnycastMessage, a SitePercMessage with
  * ttl = -1 (infinite) visits each node.
  * @see Starsky::SitePercMessage
  * @see Starsky::AnyCastMessage
  */
class WalkAndSitePercMessage : public SitePercMessage {

    public:
	/**
	 * @param p probability that each node broadcasts the message
	 * @param walkttl the total depth from start node the walk will go
	 * @param percttl the depth the percolation will go
	 */
        WalkAndSitePercMessage(Random& r, double p, int walkttl, int percttl=-1);
	void visit(Node*, Network& aNet);
    protected:
	Random& _rand;
	AnycastMessage _ac_mes;
	double _prob;
};

}

#endif

