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

#include "node.h"
#include <iostream>
using namespace Starsky;

//Initialize the node count to zero
int Node::_node_count = 0;

Node::Node()
{
#if 1
  _name = 0;
#else
  std::stringstream s;
  s << _node_count;
  _node_count++;
  _name = s.str();
#endif
  //std::cout << "Node Constructor: " << this << " " << toString() << std::endl;
}

Node::~Node()
{
  //std::cout << "Node Destructor: " << this << std::endl;
  if( _name != 0 ) {
    delete _name;
    _name = 0;
  }
}

Node::Node(const std::string& s)
{
  _name = new std::string();
  *_name = s;
}

std::string Node::toString() const {
  if( _name != 0 ) {
    return *_name;
  }
  std::stringstream s;
  s << "n" << this;
  return s.str();
}


