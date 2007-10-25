/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida
Copyright (C) 2007  Tae Woong Choi <twchoi@ufl.edu>, University of Florida

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
#include <ran1random.h>
#include <random.h>

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
     **/
     DeetooMessage(unsigned long int r0, unsigned long int r1, bool cache, Ran1Random& r_num, double p_fail);
    /**
     * This will return all the nodes and edges in the
     * out component of a particular Node within a number of hops
     * @param anode the node to start the message at
     * @param aNet the network that the message will travel on
     * @return a network consisting of all the nodes and edges crossed in a broadcast.
     **/
    virtual DeetooNetwork* visit(Node* anode, Network& aNet);	
    void cacheItems(AddressedNode* cache_node, DeetooNetwork* o_net);	
    /**
     * This will check if node is in the range (_r0, _r1)
     **/
    bool inRange( AddressedNode* node);
    /**
     * count how many edges are visited before getting in the range
     */
    int out_edge_count;
    /**
     * count failed search during insertion
     */
    //bool insert_fail;
    /**
     * In given message, the first node to be visited in the range
     */
    AddressedNode* init_node;
  protected:
    bool _cache;   //cache or query, cache=true, query=false
    unsigned long int _mid_range;  //(_r0,_r1)/2
    unsigned long int _r0, _r1;
    unsigned long int _dist_to_lower;
    Ran1Random& _r_num;
    double _p_fail;
    /**
     * Since we implement this recursively, this function allows us to not
     * have to allocate and delete a new visited network each time.  This
     * is an optimization for speed and memory
     */
    void visit(AddressedNode* anode, Network& net_to_visit, DeetooNetwork& visited_net);
  };
	
}

#endif

