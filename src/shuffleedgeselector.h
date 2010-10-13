/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2010  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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

#ifndef starsky__shuffleedgeselector_h
#define starsky__shuffleedgeselector_h

#include "random.h"
#include <iedgeselector.h>
#include <inetworkmonitor.h>
#include <network.h>
#include <node.h>
#include <vector>

namespace Starsky {

/**
 * This class returns edges randomly selected
 * from the network.
 */
class ShuffleEdgeSelector : public IEdgeSelector, public INetworkMonitor {

  public:
    ShuffleEdgeSelector(Random& rand) : _rand(rand) { _my_net = 0; }
    virtual void selectFrom(Network* net);
    virtual void postAdd(Network* net);
    virtual void postEdgeAdd(Edge* add);
    virtual void postEdgeRemove(Edge* rem);
    virtual Edge* select();
    virtual Edge* select(Edge* avoid);

  protected:
    /**
     * Here is a list of all the edges.  We keep them
     * updated by monitoring the network (INetworkMonitor)
     * We can get random edges quickly this way
     */
    std::vector<Edge*> _edges;
    std::vector<Edge*>::iterator _pos;
    Random& _rand;
    /**
     * This is the network we select from
     */
    Network* _my_net;

};
	
}

#endif
