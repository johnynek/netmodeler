/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2010  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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

#ifndef starsky__randcompnetwork_h
#define starsky__randcompnetwork_h

#include "network.h"
#include "inetworkpartitioner.h"
#include "random.h"
#include <vector>
#include <set>

namespace Starsky {

 /**
  * Produces random networks where each edge exists with probability p
  */
class RandCompNetwork : public Network {

    public:
        /**
         * use vector<int>::resize(K, N_c) to make K clusters of size N_c
         */
        RandCompNetwork(const std::vector<int>& comp_cnt,
	                  double exp_degree, double out_p_in_p_ratio,
		          Random& rand);
        ~RandCompNetwork();
        double accuracyOfPartition(NetworkPartition* part) const;
        /** how much information does partition have about edge cluster membership
         * random variable: is edge internal or external to a cluster.
         * How much information does the partition have?
         */
        double informationOfPartition(NetworkPartition* part) const;

    protected:
        double getInCompProb(double d, int n_c, int ntot, double r) const;	
        int* _clusters;
};

}

#endif
