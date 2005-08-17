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

#include "npartitenetwork.h"

using namespace std;
using namespace Starsky;

bool NPartiteNetwork::add(int type, Node* node)
{
  _type_to_node_map[type].insert(node);
  return Network::add(node);
}

const Network::NodePSet& NPartiteNetwork::getNodes(int type) const
{
  map<int,Network::NodePSet>::const_iterator it = _type_to_node_map.find( type );
  if( _type_to_node_map.end() == it) {
      return Network::_empty_nodeset;
  }
  return it->second;
}

NPartiteNetwork& NPartiteNetwork::operator=(const NPartiteNetwork& net)
{
  this->operator=(net);
  _type_to_node_map = net._type_to_node_map;
  return *this;
}

int NPartiteNetwork::remove(Node* n)
{
  //We don't ask for the type here, but we assume that there are not
  //many types.  We are optimizing towards networks with large number
  //of nodes, but a low number of types.
  map<int, Network::NodePSet>::iterator it;
  for( it = _type_to_node_map.begin(); it != _type_to_node_map.end(); it++)
  {
    it->second.erase(n);
  }
  return Network::remove(n);
}
