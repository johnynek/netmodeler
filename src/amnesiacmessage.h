#ifndef starsky__amnesiacmessage
#define starsky__amnesiacmessage


#include "message.h"
#include "network.h"
#include "node.h"
#include "random.h"

#include <set>

namespace Starsky {

/**
 * Randomly walks through the network, but only remembers the last node it finds.
 */
class AmnesiacMessage : public Message {

    public:
        AmnesiacMessage(Random& r, int ttl);
	//This is a recursive function
        void visit(Node*, Network& aNet); 
	
    protected:
	Random& _rand;
	int _ttl;
	
};
	
}

#endif
