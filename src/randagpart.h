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

#ifndef starsky__randagpart_h
#define starsky__randagpart_h

#include "newmancom.h"
#include "random.h"

namespace Starsky {

/**
 * The NewmanCom algorithm joins communities which
 * will maximize modularity.  Here we do some randomization
 * to compare the resulting communities
 */
class RandAgPart : public NewmanCom {

  enum Method {
    /**
     * In the RandomStart mode, we choose a community at random
     * and merge it with the one that maximizes modularity
     */
    RandomStart = 1,
    /**
     * In noisy join, with probability p, we join a random pair,
     * else we join the pair that maximize modularity
     */
    NoisyJoin = 2,
    /**
     * In PrefJoin we join two communities with a probability
     * proportional to the delta_q^prob they produce (as measured from the
     * minimum delta_q).  So, to select uniformly over the joins,
     * we set prob = 0.  To select linearly proportional we set prob = 1;
     * So bias towards larger delta_q, set prob > 1, so bias towards smaller
     * delta_q, set prob < 0.
     */
    PrefJoin = 3
  };
	
  public:
    RandAgPart(Random& rand, std::string method, double prob = 0.0) : _rand(rand), _param(prob) {
      if( method == "RandomStart" ) {
        _method = RandomStart;
      }
      else if( method == "NoisyJoin" ) {
        _method = NoisyJoin;
      }
      else if( method == "PrefJoin" ) {
        _method = PrefJoin;
      }
    }
    /**
     * @param rand the Random to use
     * @param meth the Method to use 
     * @param prob the probability to use in the Method
     */
    RandAgPart(Random& rand, Method meth, double prob = 0.0);

    virtual int getCommunities(const Network& net, std::vector<double>& q,
                   std::vector< std::pair<int,int> >& joins);

    /**
     *
     * @param net the network we are working with
     * @param community_map The current mapping of node to community
     * @param e_ij the fraction of edges going from com i to com j
     * @param a_i the fraction of edges that are in com i (sum_j e_{ij})
     * @param com1 the community to merge with com2
     * @param com2 the community to merge with com1
     * @return the change in modularity
     */
    virtual double getNextJoin(const Network& net,
                               const std::map<Node*, int>& community_map,
                               const std::map<int, std::map<int, double> >& e_ij,
                               const std::vector< double >& a_i,
                               int& com1, int& com2);
  protected:
    /**
     * Return the index of a community (that has yet to be merged into another)
     */
    int getRandomCommunity(int avoid = -1);
    void initRandomCom(const Network& net);
    void removeCommunity(int com);
    
    Method _method;
    Random& _rand;
    double _param;
    //the i^{th} position holds a pointer to a node in the i^{th} community.
    //If that community has been erased, the position holds 0
    std::vector<Node*> _com_to_node;
    //This is the order in which we select communities for joining
    std::vector<int> _com_order;
    int _com_pos; //which element in _com_order is next;
	
};
	
}

#endif
