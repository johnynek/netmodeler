#ifndef starsky__anycastmessage
#define starsky__anycastmessage


#include "message.h"
#include "network.h"
#include "node.h"
#include "random.h"

#include <set>

namespace Starsky {

/**
 * Randomly percolates through a network.  This is a kind of random walker.
 * It may multiply (if possible) at each walk (say going to n randomly selected
 * nodes at each step).
 */
class AnycastMessage : public Message {

    public:
	/**
	 * @param r Random number generator to use
	 * @param max_routes the number of copies to make at each node.
	 * @param ttl max number of steps taken.  copies inherit this value.
	 */
        AnycastMessage(Random& r, int max_routes=1, int ttl = -1);
	//This is a recursive function
        void visit(Node*, Network& aNet); 
	
    protected:
	Random& _rand;
	int _max;
	int _ttl;
	
};
	
}

#endif
