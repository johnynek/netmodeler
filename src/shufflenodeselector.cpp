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

#include <shufflenodeselector.h>

using namespace Starsky;
using namespace std;

void ShuffleNodeSelector::postAdd(Network* net)
{
  _my_net = net;
  _nodes.clear();
  auto_ptr<NodeIterator> ni( net->getNodeIterator() );
  while( ni->moveNext() ) {
    _nodes.push_back( ni->current() );
  }
  _pos = _nodes.end();
}

void ShuffleNodeSelector::postNodeAdd(Node* add)
{
    //This is a new node:
    _nodes.push_back( add );
}

void ShuffleNodeSelector::postNodeRemove(Node* add)
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

Node* ShuffleNodeSelector::select()
{
  return select(0);
}

Node* ShuffleNodeSelector::select(Node* avoid)
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
      if( _pos == _nodes.end() ) {
        _rand.shuffle(_nodes);
        _pos = _nodes.begin();
      }
      ret_val = *_pos;
      ++_pos;
      if( ret_val == avoid ) { ret_val = 0; }
    }
  }
  return ret_val;
}

void ShuffleNodeSelector::selectFrom(Network* net)
{
  if( _my_net == net ) {
    //Don't do anything.
    return;
  }
  else if ( _my_net != 0 ) {
    //Stop monitoring the old network:
    _my_net->remove(this);
  }
  net->add(this);
}
