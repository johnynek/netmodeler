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

#ifndef starsky__percolationmapper_h
#define starsky__percolationmapper_h

#include <network.h>
#include <random.h>
#include <inetworkmapper.h>

namespace Starsky {

class PercolationMapper : public INetworkMapper {

  public:
    /**
     * We keep each edge with probability bond_p,
     * we keep each node with probability site_p
     */
    PercolationMapper(Random& rand, double bond_p, double site_p = 1.0);

    /**
     * Returns the expected theshold of an infinite random
     * graph with the same degree distribution as the given
     * graph.
     * @return <k>/<k(k-1)>
     */
    static double getExpectedThreshold(Network* net) {
      double k1 = net->getDegreeMoment(1);
      double k2 = net->getDegreeMoment(2);
      return (k1/(k2 - k1));
    }
    
    /**
     * This "percolates" the given network
     * modifying it in place.
     */
    virtual void map(Network* net);

  protected:
    double _bond_p;
    double _site_p;
    Random& _rand;
	
};
	
}

#endif
