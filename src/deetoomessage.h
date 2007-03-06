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

#ifndef starsky__deetoomessage_H
#define starsky__deetoomessage_H

#include <message.h>
#include <addressednode.h>
#include <deetoonetwork.h>

namespace Starsky {

/**
 * Message that does a local breadth first broadcast.  Each node rebroadcasts
 * the message to its neighbors unless it has seen it already or the TTL=0 
 * or local boundary.
 * If the TTL is -1, the broadcast goes forever.
 * This code is basically identical to the Starsky::PercolationMessage class
 * except that the percolation probability is 1.
 * 
 */
	
class DeetooMessage : public Message {

  public:
    /**
     * @param r0 lower limit of multicasting range
     * @param r1 upper limit of multicasting range
     * @param c_q cache message for caching if it's true, otherwise message for query 
     */
     DeetooMessage(unsigned long int r0, unsigned long int r1, bool cache=true);
    /**
     * This will return all the nodes and edges in the
     * out component of a particular Node within a number of hops
     * @param anode the node to start the message at
     * @param aNet the network that the message will travel on
     * @return a network consisting of all the nodes and edges crossed in a broadcast.
     */
    virtual DeetooNetwork* visit(Node* anode, Network& aNet);	
    void cacheItems(AddressedNode* cache_node, DeetooNetwork* o_net);	
    bool inRange( AddressedNode* node);
  protected:
    bool _cache;
    unsigned long int _r0, _r1;
};
	
}

#endif

