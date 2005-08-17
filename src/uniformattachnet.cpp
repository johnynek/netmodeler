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

#include "uniformattachnet.h"

using namespace Starsky;
using namespace std;

UniformAttachNet::UniformAttachNet(const Network& seed,
		             Random& r,
			     int cons) : 
			                 _connections(cons),
					 _rand(r)
{
    Network::operator=(seed);
    //Load the nodes into a vector
    _nodes.insert( _nodes.begin(), getNodes().begin(), getNodes().end() ); 
}

bool UniformAttachNet::add(Node* n)
{
  _nodes.push_back(n);
  return Network::add(n);
}

Node* UniformAttachNet::findPartnerFor(Node* n)
{
    //Don't include the neighbors:
    NodePSet::const_iterator nit;
    const NodePSet& neighbors = getNeighbors(n);
    int index = _rand.getInt( _nodes.size() - 1 );
    Node* partner = _nodes[ index ];
    int attempts = _nodes.size();
    while( ((partner == n)
           || (neighbors.find(partner) != neighbors.end() ))
	   && (attempts > 0) )
    {
      index = (index + 1) % _nodes.size();
      partner = _nodes[ index ];
      attempts--;
    }
    if( attempts == 0 ) {
      cerr << "Count not find a partner for n: " << n << endl;
    }
    return partner;
}


int UniformAttachNet::remove(Node* n)
{
  _nodes.push_back(n);
  vector<Node*>::iterator n_it = _nodes.begin();
  while( *n_it != n && (n_it != _nodes.end()) )
  {
    n_it++;
  }
  if( *n_it == n ) { _nodes.erase( n_it ); }
  return Network::remove(n);
}
