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

#ifndef starsky__callawaynetwork
#define starsky__callawaynetwork

#include "network.h"
#include "incrementable.h"
#include "node.h"
#include "edge.h"
#include "random.h"

#include <vector>


namespace Starsky {

 /**
  * @see cond-mat/0104546.
  * The protocol is simple.  Each time step, add a node, * and with
  * probability p, connect * a pair of nodes.  Note that there will
  * likely be MANY components to this graph
  */
class CallawayNetwork : public Network,
                        public Incrementable {	
    public:
        CallawayNetwork(int steps,
			double p,
			Random& rand);
	CallawayNetwork(const Network& seed,
			double p,
			Random& rand);

	/**
	 * @see Starsky::Incrementable
	 * Takes the next time step
	 */
	virtual void incrementTime(int steps=1);
	
    protected:
	/**
	 * the random number generator which selects the nodes
	 * to connect
	 */
	Random& _rand;
	/**
	 * the probability that a pair of nodes will be connected
	 * at each time step.
	 */
	double _p;
	/**
	 * a vector of all nodes.  This makes selecting a node
	 * at random an O(1) operation rather than O(N)
	 */
	std::vector<Node*> _node_vec;
};
	
}

#endif
