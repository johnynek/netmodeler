#include "prefdelcompnetwork.h"

using namespace Starsky;
using namespace std;

PrefDelCompNetwork::PrefDelCompNetwork(const Network& seed,
		                       Random& r,
				       double p,
				       int connections,
				       double q) : PrefAtNetwork(seed,
					                         r,
							         connections),
				                                 _del_p(p),
								 _restore_q(q)
{

}

PrefDelCompNetwork::PrefDelCompNetwork(const Network& seed,
		                       Random& r,
				       const DegreePreferenceFunction& dp,
				       double p,
				       int connections,
				       double q) : PrefAtNetwork(seed,
					                         r,
								 dp,
								 connections),
				                                 _del_p(p),
								 _restore_q(q)
{

}	

const Network::NodePSet PrefDelCompNetwork::getRemoveNodes()
{
  if( _rand.getBool( _del_p ) && (node_set.size() > 1)) {
    NodePSet::const_iterator nit;

    int node = _rand.getInt(node_set.size() - 1);
    nit = node_set.begin();
    while( node-- > 0) { nit++; }
    NodePSet removenodes;
    removenodes.insert(*nit);
    return removenodes;
  }
  return _empty_nodeset;
}

bool PrefDelCompNetwork::needsCompensation(Node* n, Edge* e)
{
  return _rand.getBool( _restore_q );
}
