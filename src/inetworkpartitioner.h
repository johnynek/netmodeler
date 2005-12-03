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

#ifndef starsky__inetworkpartioner_h
#define starsky__inetworkpartioner_h

#include <set>
#include <map>
#include <network.h>
#include <containernode.h>
#include <containeredge.h>

namespace Starsky {

/**
 * @todo remove this and the set<Network*> and replace it with
 * a Network of Networks.
 * This is so we can sort set<Network*> as a function of size
 */
struct networkptr_gt : public std::binary_function<Network*, Network*, bool> {
  bool operator()(Network* x, Network* y) {
    return !(*x < *y);
  }
};
	
   /**
    * This function deletes the result of a previous
    * partition.  This should be called when the
    * data is no longer used
    */
class INetworkPartitioner {

  public:
   /**
    * Delete the memory of this partition.  If you want to "keep"
    * a component network, just set that entry to 0 or remove it
    * from the list.
    */
   virtual void deletePartition(std::vector<Network*>*  part);

  /**
   * Interface which Community finding algorithms
   * can subclass
   */
   double modularityOf(std::vector<Network*>* partition, const Network& orig);
   /**
    * The caller of this function should delete this memory
    * when done (Network* and vector<>*)
    * deletePartition does this for you.
    * This vector is sorted from largest to smallest ([0] is the biggest)
    */
   virtual std::vector<Network*>* partition(const Network& input) = 0;
  

   /**
    * This is gives a hierarchical picture of the network.  Each cluster
    * becomes a node.  Two clusters have an edge between them if there
    * is an edge between any pair of nodes in the clusters
    *
    * @param orig the original network
    * @param part a partition of the network
    * @return a Network where is node is a ContainerNode<Network> and
    * each edge is a ContainerEdge<Network>.
    */
   Network* partitionAsNetwork(const Network& orig, std::vector<Network*>* part) const;
   /**
    * Uses the Frobenius norm to compute |A - B|, |A|, and |B| for
    * the communities given.  The partition matrix is an N x N matrix
    * with 1 at (i,j) if node i and node j are in the same partition.
    * The diagonal is zero for our calculation of |A|.  e.g. the network
    * with each node in its own partition has |A| = 0.
    *
    * If one set of nodes is larger than the other, the matrix is 0
    * for any missing nodes.
    */
   virtual long distance(std::vector<Network*>* A, std::vector<Network*>* B,
		        long& norm_a, long& norm_b);
    /**
     * Use the weights of the edges in computing modularity
     * The default is to ignore the weights
     */
    virtual void useWeights(bool weights) { _weighted = weights; }
  protected:
     bool _weighted;
};
	
}

#endif
