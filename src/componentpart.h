#ifndef starsky__componentpart
#define starsky__componentpart

#include "inetworkpartitioner.h"

namespace Starsky {

  /**
   * This is a pretty trivial partitioning algorithm:
   * Find all the component on the graph and put each
   * component into a seperate partition.
   */
class ComponentPart : public INetworkPartitioner {

  public:
    virtual std::set<Network*>* partition(const Network& net);
	
};
	
}

#endif
