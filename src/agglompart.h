#ifndef starsky__agglompart_h
#define starsky__agglompart_h

#include "inetworkpartitioner.h"

namespace Starsky {

  /**
   * This is an abstract class that Agglomerative Node Clustering
   * algorithms can subclass.
   */
class AgglomPart : public INetworkPartitioner {

  public:
    /**
     * implements INetworkPartitioner
     */
    virtual std::set<Network*>* partition(const Network& input);

  /*
   * The "join" number refers to the index of the node in the
   * default ordering of the node_set.  You can get a vector
   * which has the right order by doing:
   * vector<Node*> my_vec;
   * my_vec.insert(my_vec.begin(), node_set.begin(), node_set.end());
   * 
   * you can also use getCommunity to get the network that has
   * all edges BETWEEN communities removed, leaving only the
   * edges inside communities
   */
    virtual int getCommunities(const Network& net, std::vector<double>& q,
		   std::vector< std::pair<int,int> >& joins);
  /**
   * using the above, get the particular break down of the communities:
   */
    virtual std::set<Network*>* getCommunity(const Network& net, int step,
		    const std::vector< std::pair<int, int> >& joins);  

    /**
     * This is the community joining selection algorithm.
     * Subclasses can just implement this if they like.
     * @param net the network we are working with
     * @param community_map The current mapping of node to community
     * @param e_ij the fraction of edges going from com i to com j
     * @param a_i the fraction of edges that are in com i (\sum_j e_{ij})
     * @param com1 the community to merge with com2
     * @param com2 the community to merge with com1
     * @return the change in modularity
     */
    virtual double getNextJoin(const Network& net,
                               const std::map<Node*, int>& community_map,
		               const std::vector< std::vector<double> >& e_ij,
			       const std::vector< double >& a_i,
			       int& com1, int& com2) = 0;
	 
  protected:
    /**
     * Update the community map, e_ij, and a_i
     */
    void update(std::map<Node*, int>& community_map,
		std::vector< std::vector<double> >& e_ij,
		std::vector< double >& a_i, int com1, int com2);
	
};
	
}

#endif
