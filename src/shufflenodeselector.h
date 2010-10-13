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

#ifndef starsky__shufflenodeselector_h
#define starsky__shufflenodeselector_h

#include "random.h"
#include <inodeselector.h>
#include <inetworkmonitor.h>
#include <network.h>
#include <node.h>
#include <vector>

namespace Starsky {

/**
 * This class returns nodes randomly selected
 * from the network.
 */
class ShuffleNodeSelector : public INodeSelector, public INetworkMonitor {

  public:
    ShuffleNodeSelector(Random& rand) : _rand(rand) { _my_net = 0; }
    virtual void selectFrom(Network* net);
    virtual void postAdd(Network* net);
    virtual void postNodeAdd(Node* add);
    virtual void postNodeRemove(Node* rem);
    virtual Node* select();
    virtual Node* select(Node* avoid);

  protected:
    /**
     * Here is a list of all the nodes.  We keep them
     * updated by monitoring the network (INetworkMonitor)
     * We can get random nodes quickly this way
     */
    std::vector<Node*> _nodes;
    std::vector<Node*>::iterator _pos;
    Random& _rand;
    /**
     * This is the network we select from
     */
    Network* _my_net;

};
	
}

#endif
