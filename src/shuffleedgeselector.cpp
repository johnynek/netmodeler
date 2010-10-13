/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2010  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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

#include <shuffleedgeselector.h>

using namespace Starsky;
using namespace std;

void ShuffleEdgeSelector::postAdd(Network* net)
{
  _my_net = net;
  _edges.clear();
  
  auto_ptr<EdgeIterator> ni( net->getEdgeIterator() );
  ni->pushInto(_edges);
  _pos = _edges.end();
}

void ShuffleEdgeSelector::postEdgeAdd(Edge* add)
{
    //This is a new node:
    _edges.push_back( add );
}

void ShuffleEdgeSelector::postEdgeRemove(Edge* add)
{
  //Remove this node:
    vector<Edge*>::iterator nit;
    for(nit = _edges.begin(); nit != _edges.end(); nit++) {
      if( *nit == add ) {
        _edges.erase(nit);
        break;
      }
    }
}

Edge* ShuffleEdgeSelector::select()
{
  return select(0);
}

Edge* ShuffleEdgeSelector::select(Edge* avoid)
{
  Edge* ret_val = 0;
  if( _edges.size() == 0 ) {
    ret_val = 0;
  }
  else if( _edges.size() == 1 ) {
    if( _edges[0] != avoid ) {
      ret_val = _edges[0];
    }
    else {
      ret_val =  0;
    }
  }
  else {
    while( ret_val == 0 ) {
      if( _pos == _edges.end() ) {
        _rand.shuffle(_edges);
        _pos = _edges.begin();
      }
      ret_val = *_pos;
      ++_pos;
      if( ret_val == avoid ) { ret_val = 0; }
    }
  }
  return ret_val;
}

void ShuffleEdgeSelector::selectFrom(Network* net)
{
  if( net == _my_net ) {
    //Do nothing
    return;
  }
  else if( _my_net != 0 ) {
    //Stop monitoring _my_net
    _my_net->remove(this);
  }
  net->add(this);
}
