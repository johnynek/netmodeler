#ifndef starsky__percolationmessage_h
#define starsky__percolationmessage_h


#include "message.h"
#include "network.h"
#include "node.h"
#include "random.h"

#include <set>

namespace Starsky {

/**
 * Randomly percolates through a network.
 */
class PercolationMessage : public Message {

    public:
        /**
	 * @param r Random number generator
	 * @param p probability of passing to each edge
	 * @param ttl max number of passes.  Each time a pass is made this is decremented.  -1
	 * means infinity
	 */
        PercolationMessage(Random& r, double p, int ttl = -1);
        virtual void visit(Node*, Network& aNet); 
	
    protected:
	Random& _rand;
	double _prob;
	int _ttl;
};
	
}

#endif
