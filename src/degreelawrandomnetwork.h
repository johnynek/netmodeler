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

#ifndef starsky__degreelawrandomnetwork
#define starsky__degreelawrandomnetwork

#include "network.h"
#include "node.h"
#include "degreeprobabilityfunction.h"
#include "random.h"

#include <vector>
#include <set>
#include <algorithm>

namespace Starsky {

  /**
   * a random network with a degree distribution given.
   */

class DegreeLawRandomNetwork : public Network {

    public:
	/**
	 * @param nodes the size of the network
	 * @param dpf the probability function you want for the degree dist
	 * @param rand Random number generator used to randomly form edges
	 * @param indep If true, each node is assigned a degree independently
	 *   according to the dpf.  If false, then the dpf gives the FRACTION
	 *   of nodes with a given degree.  This means that if one selects a 
	 *   node at random, the probability distribution on degrees will be dpf
	 */
        DegreeLawRandomNetwork(int nodes,
			       DegreeProbabilityFunction& dpf,
			       Random& rand,
			       bool indep=false);
    protected:
        DegreeProbabilityFunction& _dpf;
	Random& _rand;
	
};
	
}

#endif
