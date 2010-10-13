/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005-2010  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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
#include <algorithm>
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

template<typename T>
struct ptr_gt {
  bool operator()(T x, T y) { 
    if( *x < *y ) {
      //x is smaller, so it is not greater:
      return false;
    }
    //We know: *y <= *x
    if( *y < *x ) {
      //*y <= *x and *y < *x, so, *x is bigger:
      return true;
    }
    else {
      // *x == *y
      return false;
    }
  }
};

/**
 * A representation of a partitioning a network
 */
class NetworkPartition {
  public:
    /*
     * part is deleted when this object is deleted
     */
    NetworkPartition(const Network& orig, std::vector<cnt_ptr<Network> >* part);
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
   const Network& asNetwork();
   /** Caller must delete this memory when done
    */
   std::map<Node*, cnt_ptr<Network> >* asMap() const;
   const std::vector<cnt_ptr<Network> >& asVector() const { return *_part; }  

   Iterator<cnt_ptr<Network> >* getComponents() const;
   /** Treat e_ij like a probability and return mutual information
    */
   double getComInformation() const;
   /** This is O(C log (N/C)) where C is number of comp, N is nodes
    * use asMap if you need this to be faster
    * returns 0 if this node is not in the partition
    */
   cnt_ptr<Network> getComponentOf(Node* n) const;
   const Network& getOriginal() const { return _orig; }
 
   double getModularity() const; 
   /** Create a new partition where the given node has been isolated
    * if n is not in the partition, return 0.
    */
   NetworkPartition* isolate(Node* n) const;
   /** Allocate a new partition the same as the current one with a join
    *  called must delete this memory when done
    * if comp1 == comp2 return 0.
    */
   NetworkPartition* join(const cnt_ptr<Network>& comp1, const cnt_ptr<Network>& comp2) const;
   /** Put this edge (and both nodes) into comp
    */
   NetworkPartition* transfer(const cnt_ptr<Network>& comp, Edge* edge) const;
   /** return the number of partitions here
    */
   int size() const;

  private:
    //Here are some vector<Network*> management functions:
    static bool remove(std::vector<cnt_ptr<Network> >* part, const cnt_ptr<Network>& com);
    static bool replace(std::vector<cnt_ptr<Network> >* part, const cnt_ptr<Network>& com1,
                        const cnt_ptr<Network>& com2);
    static void insert(std::vector<cnt_ptr<Network> >* part, const cnt_ptr<Network>& com);

    const Network& _orig;
    cnt_ptr<std::vector<cnt_ptr<Network> > > _part;
    cnt_ptr<Network> _part_as_net;
};
	
   /**
    * This function deletes the result of a previous
    * partition.  This should be called when the
    * data is no longer used
    */
class INetworkPartitioner {

  public:
   /**
    * The caller of this function should delete the result
    * when done.
    * This vector is sorted from largest to smallest ([0] is the biggest)
    */
   virtual NetworkPartition* partition(const Network& input) = 0;
  

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
