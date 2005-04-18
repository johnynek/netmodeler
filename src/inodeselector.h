#ifndef starsky__inodeselector_h
#define starsky__inodeselector_h

#include "node.h"
#include "network.h"
#include <set>

namespace Starsky {

/**
 * This is an interface which represents
 * different ways to select nodes from 
 * networks.
 *
 * For instance, a subclass might provide
 * a method to select uniformly at random
 * from the nodes, or preferentially...
 */
class INodeSelector {
  
  public:
    virtual void selectFrom(Network* net) = 0;
    virtual Node* select() = 0;
    /**
     * @param avoid a Node to avoid.  Set to 0 to not avoid any node
     * @return a node selected according
     * to this algorithm.
     */
    virtual Node* select(Node* avoid) = 0;
};
	
}

#endif
