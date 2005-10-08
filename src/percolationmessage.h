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

#ifndef starsky__percolationmessage_h
#define starsky__percolationmessage_h


#include "message.h"
#include "network.h"
#include "node.h"
#include "random.h"

#include <set>

namespace Starsky {

/**
 * Randomly bond percolates through a network.
 */
class PercolationMessage : public Message {

    public:
        /**
	 * @param r Random number generator
	 * @param p probability of passing to each edge
	 * @param ttl max number of passes.  Each time a pass is made this is decremented.  -1
	 * means infinity
	 */
        PercolationMessage(Random& r, double p, int ttl = -1);
	/**
	 * Make a network by bond percolating from a starting point
	 * @param start Node to start at
	 * @param aNet the network to visit
	 * @return a Network with all the Node and Edge objects visited
	 */
        virtual Network* visit(Node* start, Network& aNet); 
	
    protected:
	Random& _rand;
	double _prob;
	int _ttl;
};
	
}

#endif
