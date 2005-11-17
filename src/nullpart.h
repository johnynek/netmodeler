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

#ifndef starsky__nullpart_h
#define starsky__nullpart_h

#include "inetworkpartitioner.h"

namespace Starsky {

 /**
  * This is the null partition.  It does nothing at all:
  * all nodes and edges stay in the same partition.  It
  * is only here for testing purposes.
  */
  class NullPart : public INetworkPartitioner {
    public:
      virtual std::vector<Network*>* partition(const Network& net) {
        std::vector<Network*>* ret = new std::vector<Network*>();
        //Make a copy rather than cast away the constness of net
        Network* n = net.clone();
        ret->push_back( n );
        return ret;
      }
  };
	
}

#endif
