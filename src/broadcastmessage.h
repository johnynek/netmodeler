#ifndef starsky__broadcastmessage
#define starsky__broadcastmessage

#include <message.h>
#include <node.h>
#include <network.h>

namespace Starsky {

/**
 * Message that does a breadth first broadcast.  Each node rebroadcasts
 * the message to its neighbors unless it has seen it already or the TTL=0.
 * If the TTL is -1, the broadcast goes forever.
 *
 * This code is basically identical to the Starsky::PercolationMessage class
 * except that the percolation probability is 1.
 * 
 */
	
class BroadcastMessage : public Message {

  public:
    BroadcastMessage(int ttl=-1);
    /**
     * @param anode the node to start the message at
     * @param aNet the network that the message will travel on
     */
    virtual void visit(Node* anode, Network& aNet);	
  protected:
    int _ttl;
};
	
}

#endif
