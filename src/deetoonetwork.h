/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida
Copyright (C) 2005  Tae Woong Choi <twchoi@ufl.edu>, University of Florida

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

#ifndef starsky__deetooetwork_H
#define starsky__deetooetwork_H

#include "network.h"
#include "random.h"
#include "ran1random.h"
#include "addressednode.h"
//#include "swedge.h"
#include "edge.h"
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
//#define INT64
#ifdef INT64
  typedef unsigned long long my_int;
#else
  typedef unsigned long my_int;
#endif

namespace Starsky {

    /**
     * John Kleinberg model for a 1-D small world.  
     * Nodes are arranged in a ring, 
     * and with probability p(~1/d) each node is also connected to a shortcut.
     */
class DeetooNetwork : public Network {

    public:
	DeetooNetwork(Ran1Random& r);
        DeetooNetwork(int nodes, Ran1Random& r);
	/**
	 *make new empty network
	 */
	virtual DeetooNetwork* newNetwork() const { 
		return new DeetooNetwork(_r_short);}
	/**
	 *@param address return Node by address
	 */
	//AddressedNode* getNodeFromAddress(const int address) const;
	std::map<my_int, AddressedNode*> node_map;  //<address, Node>
	std::map<my_int, AddressedNode*> query_nm;  // node map for query
	/**
	 * print neighbors of each node.
	 */
	void printNetInfo(bool cache);
	/**
	 * @param bin_count the number of evenly spaced bins
	 * return a histogram evenly spaced over shourcut 
	 * neighbor distribution.
	 */
	std::vector<int> getNeighborDistHist(int bin_count=10) const;
	std::vector<my_int> getNeighborDist(bool cq);
	/**
	 *Make ring connection between nodes @param nodeMap
	 */
	void formRing(const std::map<my_int, AddressedNode*>& nodeMap);
	/**
	 *@param cache if cache net, true, else if query net, false
	 *make shortcut connection 
	 */
	void makeShortcutConnection(const std::map<my_int, AddressedNode*>& nd_map,
                                    bool cache);
	/**
	 * @param n_map node map
	 * @param t_addr calculated target address for shortcut
	 * return the closest neighbor node to the t_addr
	 */
	AddressedNode* findShortcutNode(const std::map<my_int, AddressedNode*>& n_map, my_int t_addr);
	/**
	 * calculate the distance between addr_a and addr_b,
	 * and return the distance
	 */
	my_int distanceTo(my_int addr_a, my_int addr_b);
	
	/** 
	 * @param n create new DeetooNetwork whose size is n
	 */
	void create(int n);
	/**
	 * @param nd_map, create another network for query with query address in nd_map
	 */
	void createQueryNet(const std::map<my_int, AddressedNode*>& nd_map);
       /**
	* @param cq_size column or row size for cache or query
	* returns lower and upper bound addresses within cacheing or qurying range
	*/
	std::pair<my_int, my_int> getRange(double cq_size);

	/**
	 * @param tnode target node to estimate network size
	 * @param cq caching or querying
	 * returns estimated network size (pick two neighbors, get ave distance,
	 * divide whole address size by this ave distance -> estimated net size)
	 */
	my_int guessNetSize(AddressedNode* tnode, bool cq);
	/**
	 * using distance between log_N neighbors
	 */
	my_int guessNetSizeLog(AddressedNode* tnode, bool cq);
	/**
	 * @param net_size total number of nodes in this creating network
	 * for making network with evenly distributed nodes in address space.
	 * when a new node join, maximize minimum distance to the neighbors' address by picking up two candidate addresses then finally select and address with longer minimum distance to the neighbors.
	 */
	void createEvenNet(int net_size);
    protected:
	Ran1Random& _r_short;
};
	
}

#endif
