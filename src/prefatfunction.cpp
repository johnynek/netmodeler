#include "prefatfunction.h"

using namespace Starsky;
using namespace std;


Node* PrefAtFunction::findPartnerFor(Node* n,
			const Network& net,
			Random& r,
			const DegreePreferenceFunction& dp)
{
  double pref_count = 0.0;
  const Network::NodePSet& nodes = net.getNodes();
  Network::NodePSet::const_iterator nit;
  for (nit = nodes.begin(); nit != nodes.end();nit++)
	{
    pref_count += dp.evaluate( net.getDegree(*nit) );
	}
  
	double max_pref = pref_count;

    //Don't include this node in the random selection
    if( net.has(n) ) {
      max_pref -= dp.evaluate( net.getDegree(n) );
    }
    //Don't include the neighbors:
    const Network::NodePSet& neighbors = net.getNeighbors(n);
    for( nit = neighbors.begin();
	 nit != neighbors.end();
	 nit++)
    {
      max_pref -= dp.evaluate( net.getDegree(*nit) );
    }
    double frac = r.getDouble01();
    //We are looking for a random fraction of max_pref
    double this_degree_sum = frac * max_pref;
    double this_count = 0.0;
    //Go through all the nodes selecting preferentially.
    nit = nodes.begin();
    Node* partner = 0;
    while( (nit != nodes.end()) && (this_count < this_degree_sum))
    {
      //Skip our self (n) and our neighbors
      if((*nit != n) && (neighbors.find(*nit) == neighbors.end()) ) {
        this_count += dp.evaluate( net.getDegree(*nit) );
	      partner = *nit;
      }
      nit++;
    }
    if( partner == n) { cerr << "n(" << n << ") == partner" << endl;}
    if( partner == 0) { cerr << "Could not find a partner for n: " << n << endl; }
    return partner;
}

Node* PrefAtFunction::findInPartnerFor(Node* n,
			const DirectedNetwork& net,
			Random& r,
			const DegreePreferenceFunction& dp)
{
  double pref_count = 0.0;
  const Network::NodePSet& nodes = net.getNodes();
  Network::NodePSet::const_iterator nit;
  for (nit = nodes.begin(); nit != nodes.end();nit++)
	{
    pref_count += dp.evaluate( net.getInDegree(*nit) );
	}
  
	double max_pref = pref_count;

    //Don't include this node in the random selection
    if( net.has(n) ) {
      max_pref -= dp.evaluate( net.getInDegree(n) );
    }
    double frac = r.getDouble01();
    //We are looking for a random fraction of max_pref
    double this_degree_sum = frac * max_pref;
    double this_count = 0.0;
    //Go through all the nodes selecting preferentially.
    nit = nodes.begin();
    Node* partner = 0;
    while( (nit != nodes.end()) && (this_count < this_degree_sum))
    {
      //Skip our self (n) and our neighbors
      if(*nit != n) {
        this_count += dp.evaluate( net.getInDegree(*nit) );
	      partner = *nit;
      }
      nit++;
    }
    if( partner == n) { cerr << "n(" << n << ") == partner" << endl;}
    if( partner == 0) { cerr << "Could not find a partner for n: " << n << endl; }
    return partner;
}
