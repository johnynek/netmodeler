#ifndef starsky__prefatnetwork
#define starsky__prefatnetwork


#include "network.h"
#include "simplerulenet.h"
#include "random.h"
#include "edge.h"
#include "degreepreferencefunction.h"
#include "linearpreference.h"

//This is used by one of the methods internally
#include <vector>
#include <map>

namespace Starsky {

 /**
  * constructs a preferentially attached network in the following way:
  * A node is added and connected to a fixed number of nodes, with
  * Prob(connecting to node with degree k) ~ k
  */
class PrefAtNetwork : public SimpleRuleNet {

    public:
        PrefAtNetwork(int nodes,
		      Random& r,
		      int cons = 1,
		      int seed_net_size = 10);
        
	PrefAtNetwork(int nodes,
		      Random& r,
		      const DegreePreferenceFunction& dp,
		      int cons = 1,
		      int seed_net_size = 10);
	
	PrefAtNetwork(const Network& seed,
		      Random& r,
		      int cons = 1);
	
	PrefAtNetwork(const Network& seed,
		      Random& r,
		      const DegreePreferenceFunction& dp,
		      int cons = 1);

	/**
	 * add a node to the network, and preferentially choose a number (_connections) of
	 * nodes to connect to
	 * @param new_node the new node for the network
	 * @return true unless the new_node is not new, in which case nothing happens.
	 */
	bool add(Node* new_node);
	bool add(Edge* new_edge);

	//The below are inherited. @see Starsky::SimpleRuleNet
	/**
	 * This function selects a node preferentially for connection
	 * @param start the node which is looking for another preferentially
	 * @return the node to connect to.  0 if there is no suitable node.
	 */
	virtual Node* findPartnerFor(Node* start);
	//This network always adds a fixed number of nodes
	virtual int getConnectionCount(Node* node) { return _connections; }
	virtual void postNodeAdd(Node* n) {  }
	/**
	 * We need to subclass this to update the preferences
	 * when an edge is removed
	 */
	virtual int remove(Edge* e);
	virtual int remove(Node* n);
	
    protected:

	/**
	 * compute the total preference of all nodes
	 */
	void computePrefCount();
	
	int _connections;
	LinearPreference _linear;
	const DegreePreferenceFunction& _pref;
        Random& _rand;
	
	//This is for caching the preference function for each node
        double pref_count;
	std::vector<double> pref_of_node;
	std::map<Node*, int> _idx_of_node;
	
};

}

#endif
