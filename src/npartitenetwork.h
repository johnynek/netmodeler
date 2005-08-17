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

#ifndef npartitenetwork
#define npartitenetwork

#include "network.h"
#include <map>
#include <string>

namespace Starsky {

/**
 * Represents networks where there are more than one type of
 * node.  Uses an int to indentify each type of node.  subclasses
 * should probably have some static const variables to give names
 * to the types used.
 */
	
class NPartiteNetwork : public Network {

    public:

	    /**
	     * Add a node an note the type
	     * @param type the type of node this is.
	     * @param aNode the node to add
	     * @return true if this is a new node, else false
	     */
	    virtual bool add(int type, Node* aNode);
	    /**
	     * @param type the type of node that you want to get the set of
	     * @return reference to the set of nodes of this type
	     */
	    virtual const NodePSet& getNodes(int type) const;
	    virtual const NodePSet& getNodes() const { return node_set; }
	    /**
	     * @param node the node to remove
	     * @return the number of edges removed by this removal
	     */
	    virtual int remove(Node* node); 

	    virtual NPartiteNetwork& operator=(const NPartiteNetwork& net);
	  
	    
    protected:
	    std::map<int, NodePSet> _type_to_node_map;
};
	
}
#endif
