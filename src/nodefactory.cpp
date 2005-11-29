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

#include <nodefactory.h>

using namespace Starsky;

Node* NodeFactory::create(const std::string& str_node)
{
    Node* result = getNode(str_node);
    if( result == 0 ) {
      //This is a new node:
      result = newNode(str_node);
      _str_to_node[ str_node ] = result;
    }
    return result;
}

Node* NodeFactory::getNode(const std::string& str_node)
{
    Node* result = 0;
    std::map<std::string, Node*>::iterator sit;
    sit = _str_to_node.find(str_node);
    if( sit != _str_to_node.end() ) {
      result = sit->second;
    }
    return result;
}


NamedNode* NamedNodeFactory::create(const std::string& str_node)
{
    NamedNode* result = getNode(str_node);
    if( result == 0 ) {
      //This is a new node:
      std::string* name = new std::string(str_node);
      result = new NamedNode(name);
      _str_to_node[ name ] = result;
    }
    return result;
}

NamedNode* NamedNodeFactory::getNode(const std::string& str_node)
{
    NamedNode* result = 0;
    std::map<std::string*, NamedNode*, stringptr_lt>::const_iterator sit;
    std::auto_ptr<std::string> name( new std::string(str_node) );
    sit = _str_to_node.find( name.get() );
    if( sit != _str_to_node.end() ) {
      result = sit->second;
    }
    return result;
}

