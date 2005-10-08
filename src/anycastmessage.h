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

#ifndef starsky__anycastmessage
#define starsky__anycastmessage


#include "message.h"
#include "network.h"
#include "node.h"
#include "random.h"

#include <set>

namespace Starsky {

/**
 * Randomly percolates through a network.  This is a kind of random walker.
 * It may multiply (if possible) at each walk (say going to n randomly selected
 * nodes at each step).
 */
class AnycastMessage : public Message {

    public:
	/**
	 * @param r Random number generator to use
	 * @param max_routes the number of copies to make at each node.
	 * @param ttl max number of steps taken.  copies inherit this value.
	 */
        AnycastMessage(Random& r, int max_routes=1, int ttl = -1);
	/**
	 * Create a new network containing the nodes and edges
	 * reached by visiting the given network
	 * @param start the Node to start at
	 * @param aNet the Network to visit
	 */
        virtual Network* visit(Node* start, Network& aNet); 
	
    protected:
	Random& _rand;
	int _max;
	int _ttl;
	
};
	
}

#endif
