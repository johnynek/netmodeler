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
    for(i = 0; i < seeds; i++) {
        new_node = new Node();
	Network::add( new_node );
	auto_ptr<NodeIterator> ni( getNodeIterator() );
	while( ni->moveNext() ) {
	    //We have to make sure we are not adding a loop,
	    //since the node will be in the node set
	    if( new_node != ni->current() ) {
	      Network::add( Edge( new_node, ni->current() ) );
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
    for(i = 0; i < seeds; i++) {
        new_node = new Node();
	Network::add( new_node );
	auto_ptr<NodeIterator> ni( getNodeIterator() );
	while( ni->moveNext() ) {
	    //We have to make sure we are not adding a loop,
	    //since the node will be in the node set
	    if( new_node != ni->current() ) {
	      Network::add( Edge( new_node, ni->current()) );
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
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while( ni->moveNext() ) {
      pref_count += _pref.evaluate( getDegree( ni->current() ) );
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
    auto_ptr<Network> neighbors( getNeighbors(n) );
    auto_ptr<NodeIterator> ni( getNeighborIterator(n) );
    while(ni->moveNext())
    {
      max_pref -= _pref.evaluate( getDegree( ni->current() ) );
    }
    double frac = _rand.getDouble01();
    //We are looking for a random fraction of max_pref
    double this_degree_sum = frac * max_pref;
    double this_count = 0.0;
    //Go through all the nodes selecting preferentially.
    Node* partner = 0;
    auto_ptr<NodeIterator> nj( getNodeIterator() );
    while(nj->moveNext() && (this_count < this_degree_sum))
    {
      Node* this_node = nj->current();
      //Skip our self (n) and our neighbors
      if((this_node != n) && (!neighbors->has(this_node)) ) {
        this_count += _pref.evaluate( getDegree( this_node ) );
	partner = this_node;
      }
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

