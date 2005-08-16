#ifndef starsky__inetworkpartioner_h
#define starsky__inetworkpartioner_h

#include <set>
#include <map>
#include <network.h>

namespace Starsky {

   /**
    * This function deletes the result of a previous
    * partition.  This should be called when the
    * data is no longer used
    */
class INetworkPartitioner {

  public:
   virtual void deletePartition(std::set<Network*>*  part);
  /**
   * Interface which Community finding algorithms
   * can subclass
   */
   double modularityOf(std::set<Network*>& partition, const Network& orig);
   /**
    * The caller of this function should delete this memory
    * when done (Network* and set<>*)
    */
   virtual std::set<Network*>* partition(const Network& input) = 0;
   
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
   virtual long distance(std::set<Network*>& A, std::set<Network*>& B,
		        long& norm_a, long& norm_b);
};
	
}

#endif
