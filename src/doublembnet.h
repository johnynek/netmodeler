#ifndef starsky__doublembnet
#define starsky__doublembnet


#include "network.h"
#include "simplerulenet.h"
#include "message.h"
#include "messagebuiltnetwork.h"
#include "node.h"
#include "random.h"

#include <vector>

namespace Starsky {
/**
 * Double Message Built Network.  This is kind of a weird one.
 *
 * When a node connects to a new node, that node reacts by
 * making a new edge to a node found by the same technique.
 *
 * This is mainly for simulating a local version of double preferential
 * attachment.
 */

class DoubleMBNet : public MessageBuiltNetwork {

    public:
	DoubleMBNet(const Network& seed,
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
	
};
	
}

#endif
