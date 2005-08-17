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

#ifndef starsky__uniformattachnet
#define starsky__uniformattachnet


#include "network.h"
#include "simplerulenet.h"
#include "random.h"
#include "edge.h"

//This is used by one of the methods internally
#include <vector>
#include <map>

namespace Starsky {

 /**
  * A uniform attachment network.  At each time step
  * a node comes and makes cons connections uniformly
  * selected over the number of existing nodes.
  */
class UniformAttachNet : public SimpleRuleNet {

    public:
	UniformAttachNet(const Network& seed,
		      Random& r,
		      int cons = 1);
	virtual bool add(Node* new_node);
	virtual int remove(Node* new_node);
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
	
    protected:

	int _connections;
        Random& _rand;
        std::vector<Node*> _nodes;	
};

}

#endif
