#include "percolationmapper.h"

using namespace Starsky;

PercolationMapper::PercolationMapper(Random& rand,
		                     double bp,
				     double sp) : _rand(rand),
						  _bond_p(bp),
						  _site_p(sp)
{

}

void PercolationMapper::map(Network& net)
{
  const Network::NodePSet& ns = net.getNodes();
  Network::NodePSet ndel_set;
  Network::NodePSet::const_iterator nit;
  for(nit = ns.begin(); nit != ns.end(); nit++)
  {
    //getBool returns true with probability of the argument
    if( !_rand.getBool( _site_p ) ) {
      ndel_set.insert( *nit );
    }
  }
  Network::NodePSet::iterator dnit;
  for(dnit = ndel_set.begin(); dnit != ndel_set.end(); dnit++)
  {
    //Delete these nodes:
    net.remove( *dnit );
  }
  //Free up this memory:
  ndel_set.clear();

  //Do the same thing for the edges: 
  const Network::EdgeSet& es = net.getEdges();
  Network::EdgeSet::const_iterator e_itc;
  Network::EdgeSet del_set;
  for(e_itc = es.begin(); e_itc != es.end(); e_itc++) {
    if( !_rand.getBool( _bond_p ) ) {
      //We delete this one:
      del_set.insert( *e_itc );
    }
  }
  //Actually remove the edges:
  Network::EdgeSet::iterator e_it;
  for(e_it = del_set.begin(); e_it != del_set.end(); e_it++) {
    net.remove( *e_it );
  }
}
