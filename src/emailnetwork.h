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

#ifndef starsky__emailnetwork_h
#define starsky__emailnetwork_h

#include <map>
#include <set>
#include <iostream>

#include "network.h"
#include "directedweightednetwork.h"
#include "node.h"
#include "edge.h"

namespace Starsky {

/**
 * This class is specifically designed to process the network from the paper "Scale-free topology of
 * email network" by Ebel et.al..
 */

	
class EmailNetwork : public DirectedWeightedNetwork {

    public:
  
	EmailNetwork();
	/**
	 * @param in the input file to construct the directed weighted network.  
	 */
	EmailNetwork(std::istream& in);

        //int getInternalSize(NodePSet& nodes);
	int getInternalSize();

	int getInternalOutSize();

	std::map<int, double> getInternalOutDegtoAverageStrengthMap() const;

        double getInternalOutDegStrengthCoefficient();

	double getInternalOutDegAverageStrengthCoefficient();

	std::map<double, int> getInternalNodeOutStrengthDist();

	void removeSmallComponents();
       
	void printInternalOutStrengthDist(std::ostream& out);

	void printOutDegStrengthMap(std::ostream& out);

	void removeSinkNodes();

	void setInternalNodes(NodePSet& nodes);
	void setInternalNodes();

	void setInternalOutNodes(NodePSet& nodes);
	void setInternalOutNodes();

 protected:
	NodePSet internal_node_set;
	NodePSet internal_out_node_set;

};
	
}

#endif

