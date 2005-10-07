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

#ifndef starsky__spatialnode
#define starsky__spatialnode

#include "node.h"
#include "random.h"
#include <vector>
#include <cmath>

namespace Starsky {

 /**
  * represents a node which has a position in a space.
  * We will consider a hypercube with each side of length 1.
  */
	
class SpatialNode : public Node {

    public:
	/**
	 * Create a node at a position selected uniformly
	 * from the dim dimensional cube.
	 * @param rand the Random to use
	 * @param dim the Dimension of the space
	 */
        SpatialNode(Random& rand, int dim);
	/**
	 * Create a node at the point specified by 
	 * the vector
	 */
	SpatialNode(const std::vector<double>& pos);
	
	/**
	 * @return the position of the current node
	 */
	const std::vector<double>& getPosition() const;
	/**
	 * @param aNode another spatial node
	 * @return magnitude of the euclidean distance to that node
	 */
        double getDistanceTo(SpatialNode* aNode) const;
	
    protected:
        std::vector<double> _coords;

	
};
	
}

#endif
