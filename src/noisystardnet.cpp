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

#include "noisystardnet.h"

using namespace Starsky;
using namespace std;

NoisyStarDNet::NoisyStarDNet(double p, Random& ran,
		             double del, double rest) :
                                                      _p(p),
						      _rand(ran),
						      _del(del),
						      _rest(rest)
{
    
}

Node* NoisyStarDNet::findPartnerFor(Node* s)
{
  Node* tmp = 0;
  //This network only has one node
  if( has(s) && (node_set.size() == 1) ) { return tmp; }
  //Get a random node in the network:
  NodePSet::const_iterator nit = node_set.begin();
  int r_n = _rand.getInt(node_set.size() - 1);
  if(has(s)) { r_n--; }
  while( r_n-- > 0 ) { nit++; }
  if( *nit == s ) { nit++; }
  tmp = *nit;
  //Now tmp has a random node in the network.
  if( _rand.getBool(_p) ) {
    return tmp;
  }
  //else:
  //we connect to tmp's neighbor of greatest degree:
  ConnectedNodePSet::const_iterator c_it;
  int max_degree = getDegree(tmp);
  Node* max_node = tmp;
  //Hack to randomly connect to a neighbor:
  int n = _rand.getInt( getNeighbors(tmp).size(), 1 );
  for(c_it = getNeighbors(tmp).begin();
      c_it != getNeighbors(tmp).end();
      c_it++)
  {
#if 1
      //Connect to the maximum degree that is not the start node: 
      if( (s != *c_it) && (max_degree < getDegree(*c_it)) )
      {
          max_degree = getDegree(*c_it);
	  max_node = *c_it;
      }
#else
      //The below was a hack to select an edge randomly 
      if(n-- == 0) { max_node = *c_it; break; }
#endif
  }
  return max_node;
}

const Network::NodePSet NoisyStarDNet::getRemoveNodes()
{
 if( _rand.getBool( _del ) ) {
  //Select a random node:
  int r = _rand.getInt( node_set.size() - 1 );
  NodePSet::const_iterator it = node_set.begin();
  while( r-- > 0 ) {
    it++;
  }
  NodePSet ret;
  ret.insert( *it );
  return ret;
 }
 return Network::_empty_nodeset;
}
