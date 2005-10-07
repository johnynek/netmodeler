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

#ifndef starsky__weightednetwork_h
#define starsky__weightednetwork_h

#include <map>
#include <set>
#include <iostream>

#include "network.h"
#include "node.h"
#include "edge.h"


namespace Starsky {

/**
 *  Represents a weighted network.  Methods that are specific
 *  to weighted networks are here.
 */

class WeightedNetwork : public Network {

    public:
	WeightedNetwork();
	/**
	 * @param in the input file to construct the weighted network.  
	 */
	WeightedNetwork(std::istream& in);
	/**
	 * Build a weighted network (with all edges weights=1.0) from
	 * an unweighted network
	 */
	WeightedNetwork(const Network& net);
#if 0
        /**
	 * @param edge the edge to add to the network.  if the edge is already exist, increment its weight by one
	 * @return true if an edge was added successfully, false otherwise
	 */
	virtual bool add(Edge* edge);	
#endif
	/**
	 * We override the non-weight based community finding
	 * algorithm from Network
	 */
        virtual int getCommunities(std::vector<double>& q,
			   std::vector< std::pair<int,int> >& joins) const;
	/**
	 * @return for each # of weights, the number of edges which have that weight
	 */
	std::map<double, int> getEdgeWeightDist(const EdgeSet& edges) const;

	std::map<double, int> getEdgeWeightDist() const;
	virtual WeightedNetwork* newNetwork() const { return new WeightedNetwork(); }
	/**
	 * Prints all the edges in the network and the weights associated with each edge.
	 */
	void printWeights(std::ostream& out) const;
    protected:
	virtual void readFrom(std::istream& in);

};
	
}

#endif


