#ifndef starsky__simkinnetwork
#define starsky__simkinnetwork

#include "network.h"
#include "node.h"
#include "edge.h"
#include "random.h"

#include <vector>


namespace Starsky {
 /**
  * Take N nodes, select n of them.  Connect each pair with probability
  * p.  Repeat this m times.
  */

class SimkinNetwork : public Network {	
    public:
        SimkinNetwork(int nodes,
			int group_size,
			double p,
			Random& rand,
			int repeat = -1);	

    protected:
	Random& _rand;
	double _p;
	//We need to have random access to the nodes, so...
	std::vector<Node*> _node_vec;
	void create(int n, int group_size, int rep);
};
	
}

#endif
