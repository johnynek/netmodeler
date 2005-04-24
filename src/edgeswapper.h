#ifndef starsky__edgeswapper_h
#define starsky__edgeswapper_h

#include "random.h"
#include <inetworkmapper.h>
#include <network.h>
#include <edgefactory.h>

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

  protected:
    Random& _rand;
    double _p;
    EdgeFactory& _ef;
    Method _method;
};
	
}

#endif
