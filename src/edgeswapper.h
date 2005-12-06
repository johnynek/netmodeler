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

#ifndef starsky__edgeswapper_h
#define starsky__edgeswapper_h

#include "random.h"
#include <inetworkmapper.h>
#include <network.h>
#include <edgefactory.h>
#include <filterediterator.h>

namespace Starsky {

/**
 * This mapper goes through all the edges.  With probability
 * p, it adds the edge to a list of edges to be swapped.
 * Then, it shuffles the list, and takes pairs of edges
 * from the list.  It reconnects the edges in one of
 * three modes defined by the enum.
 */
	
class EdgeSwapper : public INetworkMapper {
  
  public:
  enum Method {
    //RandomSwap: (default)
    RandomSwap = 1,
    //HighToHigh: the highest degree from one edge with
    //the highest degree of the other
    HighToHigh = 2,
    //HighToLow: the highest degree from one edge with
    //the lowest degree of the other
    HighToLow = 3
  };

  EdgeSwapper(EdgeFactory& ef, Random& rand, double p = 1.0, Method meth = RandomSwap);
  EdgeSwapper(EdgeFactory& ef, Random& rand, double p, std::string method) : _ef(ef), _rand(rand), _p(p)
  {
    if( method == "RandomSwap" ) {
      _method = RandomSwap;
    }
    else if( method == "HighToHigh" ) {
      _method = HighToHigh;
    }
    else if( method == "HighToLow" ) {
      _method = HighToLow;
    }
  };
  
    virtual void map(Network* net);
    /**
     * Swap the edges given by the iterator
     */
    void map(Network* net, Iterator<Edge*>* it);

  protected:
    Random& _rand;
    double _p;
    EdgeFactory& _ef;
    Method _method;
};
	
}

#endif
