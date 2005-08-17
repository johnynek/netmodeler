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

#ifndef starsky__vazquezdirectednetwork
#define starsky__vazquezdirectednetwork

#include "random.h"
#include "node.h"
#include "directededge.h"
#include "directednetwork.h"
#include "incrementable.h"

#include <vector>
#include <set>
#include <map>

namespace Starsky {

 /**
  * @see cond-mat/0105031
  * Model for citation networks and possibly software dependency networks.
  * The model works in the following way: As a node is added, it randomly
  * selects a node to connect to.  If a link is added to a node, with
  * probability p, a link is also added to all its neighbors.
  */
class VazquezDirectedNetwork : public DirectedNetwork,
                               public Incrementable {

    public:
        VazquezDirectedNetwork(int nodes, double p, Random& r);

    protected:
	/**
	 * add a node to the network, and do the connections.
	 */
	virtual void incrementTime(int steps = 1);
	Random& _rand;
	double _p;
	std::vector<Node*> _node_vec;
	
};
	
}

#endif
