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

#ifndef starsky__edgerewirer_h
#define starsky__edgerewirer_h

#include <inetworkmapper.h>
#include <edgefactory.h>
#include <inodeselector.h>
#include <random.h>

namespace Starsky {

/**
 * This class rewires each edge with probability p.
 * By rewire, we mean that the edge is removed from
 * the network, and then we select two nodes
 * in the network to add an edge between.
 *
 * The total number of edges is conserved (and thus,
 * mean degree), but the network will approach an
 * Erdos-Renyi graph as p->1
 */
class EdgeRewirer : public INetworkMapper {

  public:
    /**
     * This uses the UniformNodeSelector
     */
    EdgeRewirer(EdgeFactory& ef, double p, Random& rand);
    /**
     * These node selectors are deleted when this object
     * is deleted
     * @param startsel the selector for the start node
     * @param endsel the selector for the end node (if =0 use startsel)
     */
    EdgeRewirer(EdgeFactory& ef, double p, Random& rand,
		INodeSelector* startsel,
		INodeSelector* endsel=0);
    ~EdgeRewirer();
    
    /**
     * Rewire the edges in this network
     */
    virtual void map(Network* net);
  protected:
    EdgeFactory& _ef;
    Random& _rand;
    double _prob;
    INodeSelector* _startsel;
    INodeSelector* _endsel;
};

}

#endif
