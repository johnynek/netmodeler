#ifndef starsky__uniformattachnet
#define starsky__uniformattachnet


#include "network.h"
#include "simplerulenet.h"
#include "random.h"
#include "edge.h"

//This is used by one of the methods internally
#include <vector>
#include <map>

namespace Starsky {

 /**
  * A uniform attachment network.  At each time step
  * a node comes and makes cons connections uniformly
  * selected over the number of existing nodes.
  */
class UniformAttachNet : public SimpleRuleNet {

    public:
	UniformAttachNet(const Network& seed,
		      Random& r,
		      int cons = 1);
	virtual bool add(Node* new_node);
	virtual int remove(Node* new_node);
	//The below are inherited. @see Starsky::SimpleRuleNet
	/**
	 * This function selects a node preferentially for connection
	 * @param start the node which is looking for another preferentially
	 * @return the node to connect to.  0 if there is no suitable node.
	 */
	virtual Node* findPartnerFor(Node* start);
	//This network always adds a fixed number of nodes
	virtual int getConnectionCount(Node* node) { return _connections; }
	virtual void postNodeAdd(Node* n) {  }
	
    protected:

	int _connections;
        Random& _rand;
        std::vector<Node*> _nodes;	
};

}

#endif
