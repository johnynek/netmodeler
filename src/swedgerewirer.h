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

#ifndef starsky__SWedgerewirer_h
#define starsky__SWedgerewirer_h

#include <inetworkmapper.h>
#include <edgefactory.h>
#include <inodeselector.h>
#include <random.h>
#include <swnetwork.h>
#include <randaddrnode.h>
#include <set>

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
//class SWEdgeRewirer : public INetworkMapper {
class SWEdgeRewirer {

  public:
  /**
  enum Method {
      //Rewire both end nodes of selected edge to randomly selected nodes
      BothEnd = 1,
      //Fix one end and rewire the other node of selected edge to a randomly selected node
      OneEnd = 2,
      //Fix one end and rewire the other node to a shortcut neighbor
      ToShortcut = 3
  };
  */
    /**
     * This uses the UniformNodeSelector
     */
    SWEdgeRewirer(EdgeFactory& ef, double p, Ran1Random& rand);
    /**
     * These node selectors are deleted when this object
     * is deleted
     * @param startsel the selector for the start node
     * @param endsel the selector for the end node (if =0 use startsel)
     */
    SWEdgeRewirer(EdgeFactory& ef, double p, Ran1Random& rand,
		INodeSelector* startsel,
		INodeSelector* endsel=0);
    ~SWEdgeRewirer();
    //set<Edge*> EdgesToRewire(SWNetwork* net);
    //void notToRewire(SWNetwork* net, SWNetwork* rewire_net);
    //void ToRewire(SWNetwork* net);
    /**
     * Rewire the edges in this network
     */
    //SWNetwork* map(SWNetwork* net, std::set<Edge*> edges_to_rewire, int method);
    SWNetwork* map(SWNetwork* net, int method);
    //SWNetwork* map(SWNetwork* net, SWNetwork* toRewireNet, int method);
    //std::set<Edge*> edges_to_rewire;
    //Method m;
    //SWNetwork* toRewireNet;
  protected:
    EdgeFactory& _ef;
    //Random& _rand;
    Ran1Random& _rand;
    //Method _method;
    double _prob;
    INodeSelector* _startsel;
    INodeSelector* _endsel;
    
};

}

#endif
