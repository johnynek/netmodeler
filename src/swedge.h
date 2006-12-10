/** 
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

#ifndef starsky__swedge_h
#define starsky__swedge_h

#include "edge.h"
#include "randaddrnode.h"

namespace Starsky {
  /**
   * subclass of edge
   * to store pointer to two RandAddrNodes
   */
  class SWEdge : public Edge {
    public:
      /**
       * creats an edge between start and end
       * @param attr attributes of edges
       * (e.g., DN:direct neighbor, SC:shortcut neighbor)
       */
      SWEdge (RandAddrNode* start, RandAddrNode* end, const std::string& attr );
      /** 
       * copy a swedge and return it
       */
      SWEdge* clone() const { 
	RandAddrNode* afirst = dynamic_cast<RandAddrNode*> (first);
	RandAddrNode* asecond = dynamic_cast<RandAddrNode*> (second);
	std::cout << "--------clone------------" << std::endl;
	std::cout << "(" << afirst->getAddress() << "," << asecond->getAddress() << ")" << std::endl;
        //return new SWEdge((RandAddrNode*)first, (RandAddrNode*)second, _attr); }
        return new SWEdge(afirst, asecond, _attr); }
      /** 
       * retrurns edge's attribute
       */
      virtual std::string getAttributes() const;
      std::string printAttributes() const;
      //const std::string& attr;
      int getDistanceBetween(int node_size);
      void printEdgeInfo ();
    protected:
      const std::string& _attr;
  };
}
#endif
