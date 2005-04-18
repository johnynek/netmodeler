#ifndef starsky__edgerewirer_h
#define starsky__edgerewirer_h

#include <inetworkmapper.h>
#include <edgefactory.h>
#include <inodeselector.h>
#include <random.h>

namespace Starsky {

/**
 * This class rewires each edge with probability p.
 * By rewire, we mean that the edge is removed from
 * the network, and then we select two nodes
 * in the network to add an edge between.
 *
 * The total number of edges is conserved (and thus,
 * mean degree), but the network will approach an
 * Erdos-Renyi graph as p->1
 */
class EdgeRewirer : public INetworkMapper {

  public:
    /**
     * This uses the UniformNodeSelector
     */
    EdgeRewirer(EdgeFactory& ef, double p, Random& rand);
    /**
     * These node selectors are deleted when this object
     * is deleted
     * @param startsel the selector for the start node
     * @param endsel the selector for the end node (if =0 use startsel)
     */
    EdgeRewirer(EdgeFactory& ef, double p, Random& rand,
		INodeSelector* startsel,
		INodeSelector* endsel=0);
    ~EdgeRewirer();
    
    /**
     * Rewire the edges in this network
     */
    virtual void map(Network* net);
  protected:
    EdgeFactory& _ef;
    Random& _rand;
    double _prob;
    INodeSelector* _startsel;
    INodeSelector* _endsel;
};

}

#endif
