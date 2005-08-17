/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

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
