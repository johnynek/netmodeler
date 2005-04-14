#ifndef starsky__inetworkmapper_h
#define starsky__inetworkmapper_h

#include <network.h>

namespace Starsky {

/**
 * An interface that classes can implement
 * which represents some kind of in place
 * mapping of the network.  This will
 * involve the removal or addition of edges.
 *
 * If you don't want to "mess up" a Network,
 * then you must first make a copy of that
 * network, and operate on that network
 */
	
class INetworkMapper {

  public:
    virtual void map(Network& net) = 0;
	
};
	
}

#endif
