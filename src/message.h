#ifndef starsky__message_h
#define starsky__message_h


#include <set>
#include "node.h"
#include "network.h"

namespace Starsky {

/**
 * Represents visitors which move on the networking going
 * to various nodes
 * @see Starsky::Node
 * @see Starsky::Network
 */
class Message {

    public:
	virtual ~Message() { };
	/**
	 * Tells the message to forget where it has been.  Basically reset.
	 */
	void forgetVisitedNodes();
	/**
	 * @return the number of edges crossed
	 */
	int getCrossedEdgeCount() const { return _crossed_edges; }
	/**
	 * @return set of all nodes visited by this message.
	 */
	const std::set<Node*>& getVisitedNodes() const;
	/**
	 * @param aNode the node to start visiting at
	 * @param aNet the network to walk on
	 */
	virtual void visit(Node* aNode, Network& aNet)=0;

    protected:
	/**
	 * holds the set of visited nodes
	 */
	std::set<Node*> _visited_nodes;
	int _crossed_edges;
};
	
}

#endif
