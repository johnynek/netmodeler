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
};
	
}

#endif
