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

#include <swnodeselector.h>

using namespace Starsky;
using namespace std;

void SWNodeSelector::postAdd(SWNetwork* net)
{
  _my_net = net;
  _nodes.clear();
  auto_ptr<NodeIterator> ni( net->getNodeIterator() );
  while( ni->moveNext() ) {
    _nodes.push_back( ni->current() );
  }
}

void SWNodeSelector::postNodeAdd(Node* add)
{
    //This is a new node:
    _nodes.push_back( add );
}

void SWNodeSelector::postNodeRemove(Node* add)
{
  //Remove this node:
    vector<Node*>::iterator nit;
    for(nit = _nodes.begin(); nit != _nodes.end(); nit++) {
      if( *nit == add ) {
        _nodes.erase(nit);
        break;
      }
    }
}

RandAddrNode* SWNodeSelector::select()
{
  return select(0);
}

RandAddrNode* SWNodeSelector::select(Node* avoid)
{
  RandAddrNode* ret_val = 0;
  int node_pos = 0;
  if( avoid != 0 ) {
    /*
     * We are finding a shortcut for avoid
     */
    RandAddrNode* a_avoid = dynamic_cast<RandAddrNode*>(avoid);
    double x = _rand.getDouble01();
    int n = _my_net->getNodeSize();
    int k = int(pow(n, x));
    node_pos = (a_avoid->getAddress() + k) % n;
  }
  else {
    /*
     * We were given no node to avoid (avoid = 0), so just
     * pick one at random
     */
    node_pos = _rand.getInt( _nodes.size() - 1 );
  }
  ret_val = dynamic_cast<RandAddrNode*>( _nodes[node_pos] );
  return ret_val;
}
