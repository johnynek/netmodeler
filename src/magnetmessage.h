#ifndef starsky__magnetmessage
#define starsky__magnetmessage

#include "message.h"
#include "random.h"

namespace Starsky {

/**
 * A message which is passed to a random neighbor with probability
 * (1-p) and to go to the neighbor with maximum degree with
 * probability p.
 */
	
class MagnetMessage : public Message {

    public:
        MagnetMessage(Random& r, double p, int ttl=-1);
	void visit(Node* n, Network& aNet);
	
    protected:
	Random& _rand;
	double _p;
	int _ttl;
};
	
}

#endif
