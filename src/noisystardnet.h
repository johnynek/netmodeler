#ifndef starsky__noisystardnet
#define starsky__noisystardnet

#include "simplerulenet.h"
#include "node.h"
#include "random.h"

namespace Starsky {

  /**
   * This noisy star network adds a new node at each
   * time step.  The node is added by selecting a random
   * node in the network, the new node connects to the
   * neighbor of this node with the highest degree with
   * probability (1-p), and it connects to the randomly
   * selected node with probability p.
   *
   * To model deletion compensation, you can set the probability
   * that a node is deleted each time step with del_prob, and
   * the probability that a lost edge is restored with rest_prob
   */
	
class NoisyStarDNet : public SimpleRuleNet {

    public:
	NoisyStarDNet(double p, Random& ran,
		      double del_prob = 0.0,
		      double rest_prob = 0.0);

	//These are reimplemented from DynamicNetwork
	virtual Node* findPartnerFor(Node* start);
        virtual const NodePSet getRemoveNodes();

	virtual bool needsCompensation(Node* node, Edge* edge)
	{
          return _rand.getBool(_rest);
	}
    protected:
	double _p;
	Random& _rand;
	double _del;
	double _rest;
};
	
}

#endif
