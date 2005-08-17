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

#include <uniformnodeselector.h>

using namespace Starsky;
using namespace std;

void UniformNodeSelector::postAdd(Network* net)
{
  _my_net = net;
  _nodes.clear();
  _nodes.insert( _nodes.begin(), _my_net->getNodes().begin(), _my_net->getNodes().end() );
}

void UniformNodeSelector::postNodeAdd(Node* add)
{
    //This is a new node:
    _nodes.push_back( add );
}

void UniformNodeSelector::postNodeRemove(Node* add)
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

Node* UniformNodeSelector::select()
{
  return select(0);
}

Node* UniformNodeSelector::select(Node* avoid)
{
  Node* ret_val = 0;
  if( _nodes.size() == 0 ) {
    ret_val = 0;
  }
  else if( _nodes.size() == 1 ) {
    if( _nodes[0] != avoid ) {
      ret_val = _nodes[0];
    }
    else {
      ret_val =  0;
    }
  }
  else {
    while( ret_val == 0 ) {
      int rand_int = _rand.getInt( _nodes.size() - 1 );
      ret_val = _nodes[rand_int];
      if( ret_val == avoid ) { ret_val = 0; }
    }
  }
  return ret_val;
}
