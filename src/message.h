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

#ifndef starsky__message_h
#define starsky__message_h


#include "node.h"
#include "network.h"

namespace Starsky {

/**
 * Represents visitors which move on the networking going
 * to various nodes
 * @see Starsky::Node
 * @see Starsky::Network
 */
class Message {

    public:
	virtual ~Message() { };
	/**
	 * This function visits a network and then makes a new network
	 * which only contains the visited edges and nodes.  The caller
	 * is responsible for deleting this Network.
	 *
	 * @param aNode the node to start visiting at
	 * @param aNet the network to walk on
	 * @return the visited subset of the network as a new network (caller must delete)
	 */
	virtual Network* visit(Node* aNode, Network& aNet)=0;
};
	
}

#endif
