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
  if( has(s) && (getNodeSize() == 1) ) { return tmp; }
  //Get a random node in the network:
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  ni->moveNext();
  int r_n = _rand.getInt(getNodeSize() - 1);
  if(has(s)) { r_n--; }
  while( r_n-- > 0 ) { ni->moveNext(); }
  if( ni->current() == s ) { ni->moveNext(); }
  tmp = ni->current();
  //Now tmp has a random node in the network.
  if( _rand.getBool(_p) ) {
    return tmp;
  }
  //else:
  //we connect to tmp's neighbor of greatest degree:
  int max_degree = getDegree(tmp);
  Node* max_node = tmp;
  //Hack to randomly connect to a neighbor:
  int n = _rand.getInt( getDegree(tmp), 1 );
  auto_ptr<NodeIterator> nj( getNeighborIterator( tmp ) );
  while(nj->moveNext()) {
      Node* this_neigh = nj->current();
#if 1
      //Connect to the maximum degree that is not the start node: 
      if( (s != this_neigh) && (max_degree < getDegree(this_neigh)) )
      {
          max_degree = getDegree(this_neigh);
	  max_node = this_neigh;
      }
#else
      //The below was a hack to select an edge randomly 
      if(n-- == 0) { max_node = this_neigh; break; }
#endif
  }
  return max_node;
}

const Network::NodePSet NoisyStarDNet::getRemoveNodes()
{
 if( _rand.getBool( _del ) ) {
  //Select a random node:
  int r = _rand.getInt( getNodeSize() - 1 );
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  ni->moveNext();
  while( r-- > 0 ) {
    ni->moveNext();
  }
  NodePSet ret;
  ret.insert( ni->current() );
  return ret;
 }
 return Network::_empty_nodeset;
}
