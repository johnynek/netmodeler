/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of Florida
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida
Copyright (C) 2005  Tae Woong Choi <twchoi@ufl.edu>, University of Florida

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
     
#ifndef starsky__addressednode_h
#define starsky__addressednode_h

#include "node.h"
#include <set>
//#define INT64
#ifdef INT64
  typedef unsigned long long my_int;
#else
  typedef unsigned long my_int;
#endif
namespace Starsky {

  /**
   * Addressed node class which every node in the network has address
   */
  //template<typename T>
  class AddressedNode : public Node {
    protected:
      /**
       * address for cache
       */
      my_int _c_address;
      /**
       * address for query 
       */
      my_int _q_address;
      std::set<std::string> _itemSet;
      //bool _own;
      my_int _dist;
      my_int _small;
      my_int _big;
       
    public:
      my_int addr_i; // column address
      my_int addr_j; // row address
      bool cache;
      AddressedNode();
      /**
       * @param addr node's address 
       * @param item object to hold
       * @param own if true, delete the item when we are deleted
       */
      AddressedNode(const my_int addr, std::set<std::string> itemSet) ;
      ~AddressedNode() {_itemSet.clear(); }
      
      /**
       * @param cache true if cache, else query
       */
      my_int getAddress(bool cache);
      /**
       *@param nodes total nodes size
       *@param target calculate ring distance to the target
       * return distance to the target
       */
      my_int getDistanceTo(my_int t_addr, bool cache);
      /**
       * return to the pointer to the object being contained.
       */
      std::set<std::string> getItem() const { return _itemSet; }
      /**
       * return true if this node has qItem.
       */
      bool searchItem( std::string qItem);
      /**
       * @param item, insert item to a node
       */
      void insertItem(std::string item);
      /**
       * @param item delete this item from a node
       */
      void deleteItem(std::string item);
    };
}
#endif
