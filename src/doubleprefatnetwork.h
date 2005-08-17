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

#ifndef starsky__doubleprefatnetwork
#define starsky__doubleprefatnetwork


#include "network.h"
#include "prefatnetwork.h"
#include "random.h"
#include "edge.h"
#include "degreepreferencefunction.h"
#include "linearpreference.h"

//This is used by one of the methods internally
#include <vector>

namespace Starsky {

 /**
  * constructs a double preferentially attached network in the following way:
  * A node is added and connected to a fixed number of nodes, with
  * Prob(connecting to node with degree k) ~ k.  Then two nodes are selected
  * preferentially and an edge is added between them.
  */
class DoublePrefAtNetwork : public PrefAtNetwork {

    public:
        DoublePrefAtNetwork(const Network& seed,
			    Random& r,
			    int connections_per_step = 1);
	
	DoublePrefAtNetwork(const Network& seed,
			    Random& r,
			    const DegreePreferenceFunction& dp,
			    int connections_per_step = 1);
	
	/**
	 * After each add, we select two nodes at randome and connect them
	 */
	virtual void postNodeAdd(Node* node);
};

}

#endif
