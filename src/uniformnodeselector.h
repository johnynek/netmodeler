#ifndef starsky__uniformnodeselector_h
#define starsky__uniformnodeselector_h

#include "random.h"
#include <inodeselector.h>
#include <inetworkmonitor.h>
#include <network.h>
#include <node.h>
#include <vector>

namespace Starsky {

/**
 * This class returns nodes randomly selected
 * from the network.
 */
class UniformNodeSelector : public INodeSelector, public INetworkMonitor {

  public:
    UniformNodeSelector(Random& rand) : _rand(rand) { }
    virtual void selectFrom(Network* net) { net->add(this); }
    virtual void postAdd(Network* net);
    virtual void postNodeAdd(Node* add, bool success);
    virtual void postNodeRemove(Node* rem, bool success);
    virtual Node* select();
    virtual Node* select(Node* avoid);

  protected:
    /**
     * Here is a list of all the nodes.  We keep them
     * updated by monitoring the network (INetworkMonitor)
     * We can get random nodes quickly this way
     */
    std::vector<Node*> _nodes;
    Random& _rand;
    /**
     * This is the network we select from
     */
    Network* _my_net;

};
	
}

#endif
