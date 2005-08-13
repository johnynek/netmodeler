#ifndef starsky__trianglemapper_h
#define starsky__trianglemapper_h

#include <network.h>
#include <inetworkmapper.h>

namespace Starsky {

/**
 * A mapper which removes all edges that are not in
 * triangles or dead-ends.
 */

class TriangleMapper : INetworkMapper {

  public:
    virtual void map(Network* net);
        
};

}

#endif
