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

#ifndef starsky__sitepercmessage_h
#define starsky__sitepercmessage_h


#include "message.h"
#include "network.h"
#include "node.h"
#include "random.h"

namespace Starsky {

/**
 * Randomly percolates through a network (using site percolation theory).
 */
class SitePercMessage : public Message {

    public:
        /**
	 * @param r Random number generator
	 * @param p probability of broadcasting to all its nodes
	 * @param ttl max number of passes.  Each time a pass is made this is decremented.  -1
	 * means infinity
	 */
        SitePercMessage(Random& r, double p, int ttl = -1);
	/**
	 * @param aNode the node to start at
	 * @param aNet the Network the message is sent on
	 * @return all the Nodes and Edges reached by this process
	 */
        virtual Network* visit(Node* aNode, Network& aNet); 
	
    protected:
	Random& _rand;
	double _prob;
	int _ttl;
};
	
}

#endif
