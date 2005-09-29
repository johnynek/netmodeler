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

#include "prefatfunction.h"

using namespace Starsky;
using namespace std;


Node* PrefAtFunction::findPartnerFor(Node* n,
			const Network& net,
			Random& r,
			const DegreePreferenceFunction& dp)
{
  double pref_count = 0.0;
  NodeIterator netit = net.getNodeIterator();
  while( netit.moveNext() ) {
    pref_count += dp.evaluate( net.getDegree( netit.current() ) );
  }
  
  double max_pref = pref_count;

    //Don't include this node in the random selection
    if( net.has(n) ) {
      max_pref -= dp.evaluate( net.getDegree(n) );
    }
    //Don't include the neighbors:
    NodeIterator* ni = net.getNeighborIterator(n);
    while( ni->moveNext() ) 
    {
      max_pref -= dp.evaluate( net.getDegree( ni->current() ) );
    }
    delete ni;
    ni = 0;
    double frac = r.getDouble01();
    //We are looking for a random fraction of max_pref
    double this_degree_sum = frac * max_pref;
    double this_count = 0.0;
    //Go through all the nodes selecting preferentially.
    Node* partner = 0;
    netit = net.getNodeIterator();
    while( ( netit.moveNext() ) && (this_count < this_degree_sum))
    {
      Node* this_node = netit.current();
      //Skip our self (n) and our neighbors
      if((this_node != n) && (net.getEdge(n,this_node) == 0) ) {
        this_count += dp.evaluate( net.getDegree(this_node) );
	partner = this_node;
      }
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
  NodeIterator netit = net.getNodeIterator();
  while( netit.moveNext() ) {
    pref_count += dp.evaluate( net.getInDegree( netit.current() ) );
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
    Node* partner = 0;
    netit = net.getNodeIterator();
    while( ( netit.moveNext() ) && (this_count < this_degree_sum))
    {
      Node* this_node = netit.current();
      //Skip our self (n) and our neighbors
      if((this_node != n) && (net.getEdge(n,this_node) == 0) ) {
        this_count += dp.evaluate( net.getInDegree(this_node) );
	partner = this_node;
      }
    }
    if( partner == n) { cerr << "n(" << n << ") == partner" << endl;}
    if( partner == 0) { cerr << "Could not find a partner for n: " << n << endl; }
    return partner;
}
