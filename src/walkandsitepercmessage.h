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

#ifndef starsky__walkandsitepercmessage
#define starsky__walkandsitepercmessage

#include "node.h"
#include "network.h"
#include "random.h"
#include "sitepercmessage.h"
#include "anycastmessage.h"
#include <set>
#include <map>

namespace Starsky {

 /**
  * Message which is both a random walker and a site percolation message.
  * First it uses the AnycastMessage to make a random walk of length ttl.
  * Then, for every node visited by the AnycastMessage, a SitePercMessage with
  * ttl = -1 (infinite) visits each node.
  * @see Starsky::SitePercMessage
  * @see Starsky::AnyCastMessage
  */
class WalkAndSitePercMessage : public SitePercMessage {

    public:
	/**
	 * @param p probability that each node broadcasts the message
	 * @param walkttl the total depth from start node the walk will go
	 * @param percttl the depth the percolation will go
	 */
        WalkAndSitePercMessage(Random& r, double p, int walkttl, int percttl=-1);
	void visit(Node*, Network& aNet);
    protected:
	Random& _rand;
	AnycastMessage _ac_mes;
	double _prob;
};

}

#endif

