#ifndef starsky__inetworkpartioner_h
#define starsky__inetworkpartioner_h

#include <set>
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
	
};
	
}

#endif
