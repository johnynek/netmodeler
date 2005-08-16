#ifndef starsky__nullpart_h
#define starsky__nullpart_h

#include "inetworkpartitioner.h"

namespace Starsky {

 /**
  * This is the null partition.  It does nothing at all:
  * all nodes and edges stay in the same partition.  It
  * is only here for testing purposes.
  */
  class NullPart : public INetworkPartitioner {
    public:
      virtual std::set<Network*>* partition(const Network& net) {
        //Make a copy rather than cast away the constness of net
        Network* n = new Network();
        *n = net;
        std::set<Network*>* ret = new std::set<Network*>();
        ret->insert( n );
        return ret;
      }
  };
	
}

#endif
