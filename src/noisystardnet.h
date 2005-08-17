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

#ifndef starsky__noisystardnet
#define starsky__noisystardnet

#include "simplerulenet.h"
#include "node.h"
#include "random.h"

namespace Starsky {

  /**
   * This noisy star network adds a new node at each
   * time step.  The node is added by selecting a random
   * node in the network, the new node connects to the
   * neighbor of this node with the highest degree with
   * probability (1-p), and it connects to the randomly
   * selected node with probability p.
   *
   * To model deletion compensation, you can set the probability
   * that a node is deleted each time step with del_prob, and
   * the probability that a lost edge is restored with rest_prob
   */
	
class NoisyStarDNet : public SimpleRuleNet {

    public:
	NoisyStarDNet(double p, Random& ran,
		      double del_prob = 0.0,
		      double rest_prob = 0.0);

	//These are reimplemented from DynamicNetwork
	virtual Node* findPartnerFor(Node* start);
        virtual const NodePSet getRemoveNodes();

	virtual bool needsCompensation(Node* node, Edge* edge)
	{
          return _rand.getBool(_rest);
	}
    protected:
	double _p;
	Random& _rand;
	double _del;
	double _rest;
};
	
}

#endif
