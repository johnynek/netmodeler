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

#ifndef starsky__prefdelcompnetwork
#define starsky__prefdelcompnetwork

#include "network.h"
#include "prefatnetwork.h"
#include "random.h"

namespace Starsky {

/**
 * This is a normal preferential attachment network, except
 * at each time step after a node is added, with probability p, a node is deleted.
 * Each node which looses a link, with probability q, will make a
 * preferential connection to restore the link.
 *
 * The name comes from Preferential Deletion Compenstation.
 */

class PrefDelCompNetwork : public PrefAtNetwork {

    public:
	PrefDelCompNetwork(const Network& seed,
			   Random& r,
			   double p=1.0,
			   int connections = 1,
			   double q=1.0);
	PrefDelCompNetwork(const Network& seed,
			   Random& r,
			   const DegreePreferenceFunction& dp,
			   double p=1.0,
			   int connections = 1,
			   double q=1.0);

	//These are implemented from SimpleRuleNet
	/**
	 * This will be zero or one node selected randomly
	 */
	virtual const NodePSet getRemoveNodes();
	/**
	 * @returns true with probability _restore_q
	 */
	virtual bool needsCompensation(Node* n, Edge* e);
    protected:
	/**
	 * the probability that we select a node for deletion
	 */
	double _del_p;
	/**
	 * the probability an edge is restored
	 */
	double _restore_q;
	
};
	
}

#endif
