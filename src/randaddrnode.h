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
     
#ifndef starsky__RandAddrNode_h
#define starsky__RandAddrNode_h

#include "node.h"
#include <vector>
#include <set>

namespace Starsky {

  /**
   * RandAddrNode class which every node in the network has address
   * This is for randomized routing(randroutingmessage).
   */

  class RandAddrNode : public Node {
    protected:
      int _address;
      int _dist;
      int _small;
      int _big;
    public:
      std::vector<RandAddrNode*> local_neighbors;
      //std::set<RandAddrNode*> local_neighbors;
      RandAddrNode();
      RandAddrNode(const int addr);
      /**
       * Return a node's address
       */
      int getAddress();
      /**
       *@param nodes total nodes size
       *@param target calculate ring distance to the target
       * return distance to the target
       */
      int getDistanceTo(int nodes, RandAddrNode* target);
      std::vector<RandAddrNode*> getLocalNeighbors();
    };
}
#endif
