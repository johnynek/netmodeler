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

#ifndef starsky__iedgeselector_h
#define starsky__iedgeselector_h

#include "edge.h"
#include "network.h"

namespace Starsky {

/**
 * This is an interface which represents
 * different ways to select edges from 
 * networks.
 *
 * For instance, a subclass might provide
 * a method to select uniformly at random
 * from the edges, or preferentially...
 */
class IEdgeSelector {
  
  public:
    virtual void selectFrom(Network* net) = 0;
    virtual Edge* select() = 0;
    /**
     * @param avoid a Node to avoid.  Set to 0 to not avoid any node
     * @return a node selected according
     * to this algorithm.
     */
    virtual Edge* select(Edge* avoid) = 0;
};
	
}

#endif
