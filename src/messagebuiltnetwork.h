#ifndef starsky__messagebuiltnetwork
#define starsky__messagebuiltnetwork


#include "network.h"
#include "simplerulenet.h"
#include "message.h"
#include "node.h"
#include "random.h"

#include <vector>

namespace Starsky {
/**
 * Network built by connecting to a random node, then
 * passing messages to that node, and connecting to the nodes
 * which the message visits.
 *
 * This can be used to simulate Gnutella formation
 */

class MessageBuiltNetwork : public SimpleRuleNet {

    public:
        MessageBuiltNetwork(int nodes,
			    Message& aMessage,
			    Random& aRand,
			    int iterations = 1);
	MessageBuiltNetwork(const Network& seed,
			    Message& aMessage,
			    Random& aRand,
			    int iterations = 1);

	/**
	 * This is from SimpleRuleNet.  If start is in the network,
	 * it sends a message from itself and connects to a randomly
	 * selected visited node.  If start is not in the network,
	 * we find a node at random, and send the message to it.
	 */
	virtual Node* findPartnerFor(Node* start);
	virtual int getConnectionCount(Node* node) { return _iter; }
#if 0
	//I need to work on a better interface for handling this:
	virtual const NodePSet getRemoveNodes();
	virtual bool needsCompensation(Node* node, Edge* edge)
	{
	  //if( getDegree(node) > 50 ) { return false; } 
          //if( getDegree(node) <= _iter || _rand.getBool(1)) { return true; }
	  return true;
	}
#endif
    protected:
	Message& _message;
	Random& _rand;
	int _iter;
	
};
	
}

#endif
