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

#include "edgeswapper.h"
#include <algorithm>

//This is pretty handy:
#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

//#define DEBUG(msg) cout << msg << endl;
#define DEBUG(msg) 

using namespace std;
using namespace Starsky;

EdgeSwapper::EdgeSwapper(EdgeFactory& ef, Random& rand, double p, Method meth) : _rand(rand),
								_p(p),
								_method(meth),
								_ef(ef)
{
  
}

void EdgeSwapper::map(Network* net)
{
  DEBUG("About to look for edges to swap");
  vector<Edge*> edges;
  auto_ptr<EdgeIterator> ei( net->getEdgeIterator() );
  while( ei->moveNext() ) {
    if ( _rand.getBool(_p) ) {
      edges.push_back( ei->current() );
    }
  }
  //This is in the STL algorithms
  random_shuffle(edges.begin(), edges.end());
  //Now take pairs and delete them:
  DEBUG("About to swap");
  vector<Edge*>::iterator dit;
  FOREACH(dit, edges) {
    Edge* one = *dit;
    dit++;
    if( dit != edges.end() ) {
      DEBUG("Not at end");
      Edge* two = *dit;
      DEBUG("About to call contains on: " << one << " " << two );
      DEBUG("One: " << one->toString() << " Two: " << two->toString());
      if( one->contains( two->first ) ||
	  one->contains( two->second ) ) {
        //It is meaningless to swap edges when they share
	//a node
	DEBUG("Edges share a node");
      }
      else {
	DEBUG("Edges don't share a node");
        Method m = _method;
	if( m == RandomSwap ) {
          if( _rand.getBool( 0.5 ) ) {
            m = HighToHigh;
	  }
	  else {
            m = HighToLow;
	  }
	}
	DEBUG("Sorting nodes");
        Node *high_1, *high_2, *low_1, *low_2;
	if( net->getDegree(one->first) > net->getDegree(one->second) ) {
          high_1 = one->first;
	  low_1 = one->second;
	}
	else {
          high_1 = one->second;
	  low_1 = one->first;
	}
	if( net->getDegree(two->first) > net->getDegree(two->second) ) {
          high_2 = two->first;
	  low_2 = two->second;
	}
	else {
          high_2 = two->second;
	  low_2 = two->first;
	}
        Edge* ea;
	Edge* eb;
	DEBUG("Creating edges");
	if( m == HighToHigh ) {
          ea = _ef.create(high_1, high_2);
	  eb = _ef.create(low_1, low_2);
	}
	else if( m == HighToLow ) {
          ea = _ef.create(high_1, low_2);
	  eb = _ef.create(high_2, low_1);
	}
	if( net->has(*ea) || net->has(*eb) ) {
	  DEBUG("Could not add");
          //Don't do anything if either of these new edges are not new.
	  delete ea;
	  delete eb;
	}
	else {
	  DEBUG("Adding the edges");
          net->add(ea);
	  net->add(eb);
	  DEBUG("deleting the old edges");
	  net->remove(one);
	  net->remove(two);
	}
      }
    }
    else {
      //That's it
      DEBUG("break out");
      break;
    }
  }
}
