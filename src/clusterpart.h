/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
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

#ifndef starsky__clusterpart_h
#define starsky__clusterpart_h

#include <inetworkpartitioner.h>
#include <filterediterator.h>
#include <componentpart.h>

namespace Starsky {

/**
 * This is a simple partitioning algorithm which removes
 * edges with low clustering coefficient, and then returns
 * the components that remain.  We keep all edges
 * to nodes of degree 1.
 *
 */
class ClusterPart : public INetworkPartitioner {
  public:
    /**
     * @param min_cc if the Edge cc is this value or less, remove the edge
     */
    ClusterPart(double min_cc = 0.0);
    /**
     * Run the partitioning algorithm on the given Network
     */
    virtual std::set<Network*>* partition(const Network& input);

  protected:
    double _min_cc;

    /**
     * This is an inner class we use with FilteredIterator to 
     * find the edges to remove
     */
    class MinCCEdgeTester {
      public:
        MinCCEdgeTester(const Network* net, double m);
        /**
         * Return true if getEdgeCC(e) <= _min_cc && minDegree(e) > 1
         * @return true if we should remove this edge
         */
        bool shouldRemove(Edge* e);
        double _m; 
        const Network* _net;
    };
};

}

#endif
