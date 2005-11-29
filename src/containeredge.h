/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
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

#ifndef starsky__containeredge_h
#define starsky__containeredge_h

#include <node.h>
#include <edge.h>
#include <directededge.h>

namespace Starsky {

/**
 * Templated class which holds pointers to other objects.
 * This allows us to put anything in a Network and use
 * it as a general container type, or do things like
 * define a ContainerEdge<Network> which allows us to make
 * networks of networks.
 */
template<typename T>
class ContainerEdge : public Edge {

  public:
    /**
     * @param item object to hold
     * @param own if true, delete the item when we are deleted.
     */
    ContainerEdge(Node* start, Node* end, T* item, bool own = true) : Edge(start, end) {
       _item = item; _own = own;
    }
    ~ContainerEdge() { if( _own ) { delete _item; } _item = 0; }
    /**
     * @return to the pointer to the object being contained.
     */
    T* get() const { return _item; }
    /**
     * If you change your mind and don't want the item to be deleted
     * when the node is deleted, call this function.
     */
    void release() { _own = false; }
  protected:
    T* _item; 
    bool _own;
};

template<typename T>
class ContainerDEdge : public DirectedEdge {

  public:
    /**
     * @param item object to hold
     * @param own if true, delete the item when we are deleted.
     */
    ContainerDEdge(Node* start, Node* end, T* item, bool own = true) : DirectedEdge(start, end) {
       _item = item; _own = own;
    }
    ~ContainerDEdge() { if( _own ) { delete _item; } _item = 0; }
    /**
     * @return to the pointer to the object being contained.
     */
    T* get() const { return _item; }
    /**
     * If you change your mind and don't want the item to be deleted
     * when the node is deleted, call this function.
     */
    void release() { _own = false; }
  protected:
    T* _item; 
    bool _own;
};

}

#endif

