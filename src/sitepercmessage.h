#ifndef starsky__sitepercmessage_h
#define starsky__sitepercmessage_h


#include "message.h"
#include "network.h"
#include "node.h"
#include "random.h"

#include <set>

namespace Starsky {

/**
 * Randomly percolates through a network (using site percolation theory).
 */
class SitePercMessage : public Message {

    public:
        /**
	 * @param r Random number generator
	 * @param p probability of broadcasting to all its nodes
	 * @param ttl max number of passes.  Each time a pass is made this is decremented.  -1
	 * means infinity
	 */
        SitePercMessage(Random& r, double p, int ttl = -1);
        virtual void visit(Node*, Network& aNet); 
	
    protected:
	Random& _rand;
	double _prob;
	int _ttl;
};
	
}

#endif
