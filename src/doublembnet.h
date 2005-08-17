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

#ifndef starsky__doublembnet
#define starsky__doublembnet


#include "network.h"
#include "simplerulenet.h"
#include "message.h"
#include "messagebuiltnetwork.h"
#include "node.h"
#include "random.h"

#include <vector>

namespace Starsky {
/**
 * Double Message Built Network.  This is kind of a weird one.
 *
 * When a node connects to a new node, that node reacts by
 * making a new edge to a node found by the same technique.
 *
 * This is mainly for simulating a local version of double preferential
 * attachment.
 */

class DoubleMBNet : public MessageBuiltNetwork {

    public:
	DoubleMBNet(const Network& seed,
			    Message& aMessage,
			    Random& aRand,
			    int iterations = 1);

	/**
	 * This is from SimpleRuleNet.  If start is in the network,
	 * it sends a message from itself and connects to a randomly
	 * selected visited node.  If start is not in the network,
	 * we find a node at random, and send the message to it.
	 */
	virtual Node* findPartnerFor(Node* start);
	virtual int getConnectionCount(Node* node) { return _iter; }
#if 0
	//I need to work on a better interface for handling this:
	virtual const NodePSet getRemoveNodes();
	virtual bool needsCompensation(Node* node, Edge* edge)
	{
	  //if( getDegree(node) > 50 ) { return false; } 
          //if( getDegree(node) <= _iter || _rand.getBool(1)) { return true; }
	  return true;
	}
#endif
	
};
	
}

#endif
