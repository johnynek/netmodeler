#ifndef starsky__newmancom_h
#define starsky__newmancom_h

#include "agglompart.h"

namespace Starsky {

    /**
     * This is the Newman community finding algorithm to find
     * communities: @see cond-mat/0309508
     */
class NewmanCom : public AgglomPart {

  public:

    virtual int getCommunities(const Network& net, std::vector<double>& q,
		   std::vector< std::pair<int,int> >& joins) {
      return AgglomPart::getCommunities(net,q,joins);
    }

    /** 
     * At each step, select the two communities to join
     * the maximizes the change in modularity
     * 
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
			       int& com1, int& com2);
};
	
}

#endif
