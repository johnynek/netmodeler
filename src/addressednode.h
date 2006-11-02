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
     
#ifndef starsky__AddressedNode_h
#define starsky__AddressedNode_h

#include "node.h"

namespace Starsky {

  /**
   * Addressed node class which every node in the network has address
   */

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
      int _dist;
      int _small;
      int _big;
    public:
      unsigned long int addr_i; // column address
      unsigned long int addr_j; // row address
      AddressedNode();
      AddressedNode(const unsigned long int addr);
      /**
       * Return a node's address in cache space
       */
      unsigned long int getCacheAddress();
      /**
       * Return a node's address in cache space
       */
      unsigned long int getQueryAddress();
      /**
       *@param nodes total nodes size
       *@param target calculate ring distance to the target
       * return distance to the target
       */
      int getDistanceTo(int nodes, AddressedNode* target);
    };
}
#endif
