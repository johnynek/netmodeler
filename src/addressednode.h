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
      unsigned long int _c_address;
      /**
       * address for query 
       */
      unsigned long int _q_address;
      std::set<std::string> _itemSet;
      //bool _own;
      unsigned long int _dist;
      unsigned long int _small;
      unsigned long int _big;
       
    public:
      unsigned long int addr_i; // column address
      unsigned long int addr_j; // row address
      bool cache;
      AddressedNode();
      /**
       * @param addr node's address 
       * @param item object to hold
       * @param own if true, delete the item when we are deleted
       */
      AddressedNode(const unsigned long int addr, std::set<std::string> itemSet) ;
      ~AddressedNode() {_itemSet.clear(); }
      
      /**
       * @param cache true if cache, else query
       */
      unsigned long int getAddress(bool cache);
      /**
       * Return a node's address in cache space
       */
      //unsigned long int getCacheAddress();
      /**
       * Return a node's address in cache space
       */
      //unsigned long int getQueryAddress();
      /**
       *@param nodes total nodes size
       *@param target calculate ring distance to the target
       * return distance to the target
       */
      unsigned long int getDistanceTo(unsigned long int t_addr, bool cache);
      /**
       * return to the pointer to the object being contained.
       */
      std::set<std::string> getItem() const { return _itemSet; }
      /**
       * return true if this node has qItem.
       */
      bool searchItem( std::string qItem);
      /**
       * If you change your mind and don't want the item to be deleted
       * when the node is deleted, call this function.
       */
      void insertItem(std::string item);
      void deleteItem(std::string item);
      //void release() { _own = false; }
    };
}
#endif
