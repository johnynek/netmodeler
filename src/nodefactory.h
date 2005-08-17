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

#ifndef starsky__nodefactory_h
#define starsky__nodefactory_h

#include <node.h>
#include <map>

namespace Starsky {

/**
 * Base class for NodeFactory objects.
 * This class creates nodes, or deserializes
 * them from strings.
 *
 */
class NodeFactory {

  public:
    /**
     * This method returns the "next" node.
     * By default, it just returns new Node()
     */
    virtual Node* create() { return newNode(); }
    /**
     * This method deserializes a node.
     * By default it may also keep a table
     * of string -> Node* mappings so that
     * that repeated calls of create(string)
     * will return the same pointer for
     * the same string
     */
    virtual Node* create(const std::string& node);

    /**
     * @param node String representation of the node.
     * If we have not created this node yet, this function
     * returns 0
     */
    virtual Node* getNode(const std::string& node);
    
  protected:
    /**
     * Return the same as new Node() for
     * whatever derived type we have of Node
     */
    virtual Node* newNode() { return new Node(); }
    virtual Node* newNode(const std::string& node) { return new Node(node); }
    
    std::map<std::string, Node*> _str_to_node;
};

}

#endif
