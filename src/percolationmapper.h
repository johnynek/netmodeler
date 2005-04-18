#ifndef starsky__percolationmapper_h
#define starsky__percolationmapper_h

#include <network.h>
#include <random.h>
#include <inetworkmapper.h>

namespace Starsky {

class PercolationMapper : public INetworkMapper {

  public:
    /**
     * We keep each edge with probability bond_p,
     * we keep each node with probability site_p
     */
    PercolationMapper(Random& rand, double bond_p, double site_p = 1.0);
    /**
     * This "percolates" the given network
     * modifying it in place.
     */
    virtual void map(Network* net);

  protected:
    double _bond_p;
    double _site_p;
    Random& _rand;
	
};
	
}

#endif
