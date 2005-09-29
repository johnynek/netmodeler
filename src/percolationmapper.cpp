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

#include "percolationmapper.h"

using namespace Starsky;

PercolationMapper::PercolationMapper(Random& rand,
		                     double bp,
				     double sp) : _rand(rand),
						  _bond_p(bp),
						  _site_p(sp)
{

}

void PercolationMapper::map(Network* net)
{
  Network::NodePSet ndel_set;
  NodeIterator ni = net->getNodeIterator();
  while( ni.moveNext() ) 
  {
    Node* this_node = ni.current();
    //getBool returns true with probability of the argument
    if( !_rand.getBool( _site_p ) ) {
      ndel_set.insert( this_node );
    }
  }
  Network::NodePSet::iterator dnit;
  for(dnit = ndel_set.begin(); dnit != ndel_set.end(); dnit++)
  {
    //Delete these nodes:
    net->remove( *dnit );
  }
  //Free up this memory:
  ndel_set.clear();

  //Do the same thing for the edges: 
  Network::EdgeSet del_set;
  EdgeIterator ei = net->getEdgeIterator();
  while( ei.moveNext() ) {
    Edge* this_edge = ei.current();
    if( !_rand.getBool( _bond_p ) ) {
      //We delete this one:
      del_set.insert( this_edge );
    }
  }
  //Actually remove the edges:
  Network::EdgeSet::iterator e_it;
  for(e_it = del_set.begin(); e_it != del_set.end(); e_it++) {
    net->remove( *e_it );
  }
}
