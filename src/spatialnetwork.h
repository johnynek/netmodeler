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

#ifndef starsky__spatialnetwork
#define starsky__spatialnetwork

#include "spatialnode.h"
#include "network.h"
#include "random.h"
#include <queue>
#include <map>

namespace Starsky {

 /**
  * makes a network where the nodes are points are uniformly
  * distributed (in a given n-dimensional hypercube), and
  * then each node is connected to it's k nearest (in the space)
  * nodes.
  */
	
class SpatialNetwork : public Network {

    public:
        SpatialNetwork(int nodes,
		       Random& rand,
		       int connections = 1,
		       int dimension = 2);
    protected:
	int _dim;
	int _cons;
	
};
	
}

#endif
