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

#include "doublembnet.h"

using namespace Starsky;
using namespace std;

DoubleMBNet::DoubleMBNet(const Network& seed,
		                         Message& aMessage,
					 Random& aRand,
		                         int iterations)
                                         :  MessageBuiltNetwork(seed,
							        aMessage,
								aRand,
								iterations)
{

}

Node* DoubleMBNet::findPartnerFor(Node* start)
{
  Node* target = MessageBuiltNetwork::findPartnerFor(start);
  /**
   * When we find a node, that node also looks for a new neighbor
   */
  if( target != 0 ) {
    Node* target2 = MessageBuiltNetwork::findPartnerFor(target);
    if( target2 != 0 ) {
      add( Edge(target, target2) );
    }
  }
  return target;
}

#if 0
//This needs to be done carefully

const Network::NodePSet MessageBuiltNetwork::getRemoveNodes()
{
  if( _rand.getBool(0.5) ) {
    NodePSet rem;
    NodePSet::const_iterator nit = node_set.begin();
    if( _rand.getBool( 0.0 ) ) {
      //Remove the highest degree node
      Node* max = *nit;
      while( nit != node_set.end() ) {
        if( getDegree(max) < getDegree(*nit) ) {
          max = *nit;
        }
        nit++;
      }
      rem.insert(max);
    }
    else {
      //Return a random node:
      int rn = _rand.getInt( node_set.size() - 1);
      while( rn-- > 0 ) {
        nit++;
      }
    }

    rem.insert(*nit);
    return rem;
  }
  return _empty_nodeset;
}
#endif
