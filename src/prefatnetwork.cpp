#include "prefatnetwork.h"

using namespace Starsky;
using namespace std;

PrefAtNetwork::PrefAtNetwork(int nodes,
			     Random& r,
			     int cons,
			     int seeds) :
                                              _connections(cons),
					      _linear(),
                                              _pref(_linear),
					      _rand(r)
{
    //Make a totally connected network of size seeds:
    Node* new_node;
    int i;
    NodePSet::const_iterator n_it;
    for(i = 0; i < seeds; i++) {
        new_node = new Node();
	Network::add( new_node );
	for(n_it = node_set.begin(); n_it != node_set.end(); n_it++) {
	    //We have to make sure we are not adding a loop,
	    //since the node will be in the node set
	    if( new_node != *n_it ) {
	      Network::add( Edge( new_node, *n_it) );
	    }
	}
    }
    //Sum the preference count:
    computePrefCount();
    //Move forward the rest of the number of times:
    incrementTime( nodes - seeds ); 
}

PrefAtNetwork::PrefAtNetwork(int nodes,
			     Random& r,
			     const DegreePreferenceFunction& dp,
			     int cons,
			     int seeds) :
					      _connections(cons),
					      _linear(),
                                              _pref(dp),
					      _rand(r)
{
    //Make a totally connectioned network of size seeds:
    Node* new_node;
    int i;
    NodePSet::const_iterator n_it;
    for(i = 0; i < seeds; i++) {
        new_node = new Node();
	Network::add( new_node );
	for(n_it = node_set.begin(); n_it != node_set.end(); n_it++) {
	    //We have to make sure we are not adding a loop,
	    //since the node will be in the node set
	    if( new_node != *n_it ) {
	      Network::add( Edge( new_node, *n_it) );
	    }
	}
    }
    //Compute the pref_count
    computePrefCount();
    //Move forward the correct number of times:
    incrementTime( nodes - seeds ); 
}

PrefAtNetwork::PrefAtNetwork(const Network& seed,
		             Random& r,
			     int cons) : 
			                 _connections(cons),
			                 _linear(),
					 _pref(_linear),
					 _rand(r)
{
    Network::operator=(seed);
    computePrefCount();

}

PrefAtNetwork::PrefAtNetwork(const Network& seed,
		             Random& r,
			     const DegreePreferenceFunction& dp,
			     int cons) : 
			                 _connections(cons),
			                 _linear(),
					 _pref(dp),
					 _rand(r)
{
    Network::operator=(seed);
    computePrefCount();
}

bool PrefAtNetwork::add(Node* new_node) {

    
    if( has(new_node) == true ) { return false; }
   
    Network::add( new_node );
    //Update preference for a zero degree node:
    double pref_of_zero = _pref.evaluate(0);
    pref_count += pref_of_zero;

    return true;
}

bool PrefAtNetwork::add(Edge* new_edge) {
  double pref_a = _pref.evaluate( getDegree(new_edge->first) );
  double pref_b = _pref.evaluate( getDegree( new_edge->second ) );
  bool res = Network::add(new_edge);
  if( res ) {
    //update the preferences:
    pref_count -= ( pref_a + pref_b );
    pref_a = _pref.evaluate( getDegree( new_edge->first ) );
    pref_b = _pref.evaluate( getDegree( new_edge->second ) );
    pref_count += ( pref_a + pref_b );
  }
  if( !res ) { cerr << "Edge(" << new_edge->first << ", " << new_edge->second
	            << ") failed to add" << endl;
               if(has((*new_edge))) {cerr<<"already have that edge"<<endl; }}
  return res;
}

void PrefAtNetwork::computePrefCount()
{
    pref_count = 0.0;
    NodePSet::const_iterator nit;
    for(nit = node_set.begin();
	nit != node_set.end();
	nit++)
    {
      pref_count += _pref.evaluate( getDegree(*nit) );
    }
}

Node* PrefAtNetwork::findPartnerFor(Node* n)
{
    double max_pref = pref_count;

    //Don't include this node in the random selection
    if( has(n) ) {
      max_pref -= _pref.evaluate( getDegree(n) );
    }
    //Don't include the neighbors:
    NodePSet::const_iterator nit;
    const NodePSet& neighbors = getNeighbors(n);
    for( nit = neighbors.begin();
	 nit != neighbors.end();
	 nit++)
    {
      max_pref -= _pref.evaluate( getDegree(*nit) );
    }
    double frac = _rand.getDouble01();
    //We are looking for a random fraction of max_pref
    double this_degree_sum = frac * max_pref;
    double this_count = 0.0;
    //Go through all the nodes selecting preferentially.
    nit = node_set.begin();
    Node* partner = 0;
    while( (nit != node_set.end()) && (this_count < this_degree_sum))
    {
      //Skip our self (n) and our neighbors
      if((*nit != n) && (neighbors.find(*nit) == neighbors.end()) ) {
        this_count += _pref.evaluate( getDegree(*nit) );
	partner = *nit;
      }
      nit++;
    }
    if( partner == n) { cerr << "n(" << n << ") == partner" << endl;}
    if( partner == 0) { cerr << "Could not find a partner for n: " << n << endl; }
    return partner;
}

int PrefAtNetwork::remove(Edge* e)
{
  int ret = 0;
  if(e) {
    Node* a = e->first;
    Node* b = e->second;
    double pref_a = _pref.evaluate( getDegree(a) );
    double pref_b = _pref.evaluate( getDegree(b) );
    ret += Network::remove(e);
    if( ret != 0) {
      //update the preferences:
      pref_count -= ( pref_a + pref_b );
      pref_a = _pref.evaluate( getDegree(a) );
      pref_b = _pref.evaluate( getDegree(b) );
      pref_count += ( pref_a + pref_b );
    }
  }
  return ret;
}

int PrefAtNetwork::remove(Node* node) {
    
    bool existed = has(node);   
    //When we remove a node, each of its edges is removed.
    //This accounts for the pref_count.
    int ret = Network::remove(node);
    //Now, the preference of the node is the preference of a zero degree node:
    if(existed) {
      pref_count -= _pref.evaluate( 0 );
    }
    return ret;
}

