#ifndef starsky__mergenetwork
#define starsky__mergenetwork

#include "network.h"
#include "incrementable.h"
#include "node.h"
#include "edge.h"
#include "random.h"

#include <vector>

namespace Starsky {

 /**
  * @see http://xxx.lanl.gov/abs/nlin.AO/0403006
  * Implements the uniform merge model.  The number of nodes
  * in this network is static.
  */

class MergeNetwork : public Network, Incrementable {

  public:
	  /**
	   * @param seed the initial state of the network
	   * @param r the RNG for this network
	   * @param cons the number of connections nodes make when merged
	   */
	  MergeNetwork(const Network& seed,
                       Random& r,
		       int cons = 1);

	  /**
	   * Merge a random node with a random node
	   * at each time step.  The merged node
	   * should get cons new connections to random
	   * nodes
	   */
          virtual void incrementTime(int steps=1);

  protected:
	Random& _rand;
	/**
         * a vector of all nodes.  This makes selecting a node
         * at random an O(1) operation rather than O(N)
	 */
	std::vector<Node*> _node_vec;
	int _cons;
};

}

#endif
