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
	//virtual Network* newNetwork() const { 
		return new DeetooNetwork(_r_short);}
		//return new Network();}
	/**
	 *@param address return Node by address
	 */
	//AddressedNode* getNodeFromAddress(const int address) const;
	//std::vector<AddressedNode*> node_vec;
	std::map<unsigned long int, AddressedNode*> node_map;
	std::map<unsigned long int, AddressedNode*> query_nm;
	/**
	 * print neighbors of each node into "output" file.
	 */
	void printNetInfo(bool cache);
	/**
	 * @param bin_count the number of evenly spaced bins
	 * return a histogram evenly spaced over shourcut 
	 * neighbor distribution.
	 */
	std::vector<int> getNeighborDistHist(int bin_count=10) const;
	//bool compareNodes(const AddressedNode* a, const AddressedNode* b);
	//bool isIn(std::vector<AddressedNode*> n_vec, unsigned long int nd_addr);
	/**
	 *Make ring connection between nodes @param nodeMap
	 */
	void formRing(std::map<unsigned long int, AddressedNode*> nodeMap);
	/**
	 *@param cache if cache net, true, else if query net, false
	 *make shortcut connection 
	 */
	void makeShortcutConnection(std::map<unsigned long int, AddressedNode*> nd_map, bool cache);
	/**
	 * @param n_map node map
	 * @param t_addr calculated target address for shortcut
	 * return the closest neighbor node to the t_addr
	 */
	AddressedNode* findShortcutNode(std::map<unsigned long int, AddressedNode*> n_map, unsigned long int t_addr);
	//unsigned long int findSCAQuery(unsigned long int t_addr);
	/**
	 * calculate the distance between addr_a and addr_b,
	 * and return the distance
	 */
	unsigned long int distanceTo(unsigned long int addr_a, unsigned long int addr_b);
	
	/** 
	 * @param n create new DeetooNetwork whose size is n
	 */
	void create(int n);
	void createQueryNet(std::map<unsigned long int, AddressedNode*> nd_map);
	//void cacheItem(std::string content, AddressedNode* cn);
       // Network* queryForContent(AddressedNode* content, NodeIterator* ni);	
    protected:
	//double _p;
	Ran1Random& _r_short;
	//Random& _r_int;
};
	
}

#endif
