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

#ifndef starsky__mergenetwork
#define starsky__mergenetwork

#include "network.h"
#include "incrementable.h"
#include "node.h"
#include "edge.h"
#include "random.h"

#include <vector>

namespace Starsky {

 /**
  * @see http://xxx.lanl.gov/abs/nlin.AO/0403006
  * Implements the uniform merge model.  The number of nodes
  * in this network is static.
  */

class MergeNetwork : public Network, Incrementable {

  public:
	  /**
	   * @param seed the initial state of the network
	   * @param r the RNG for this network
	   * @param cons the number of connections nodes make when merged
	   */
	  MergeNetwork(const Network& seed,
                       Random& r,
		       int cons = 1);

	  /**
	   * Merge a random node with a random node
	   * at each time step.  The merged node
	   * should get cons new connections to random
	   * nodes
	   */
          virtual void incrementTime(int steps=1);

  protected:
	Random& _rand;
	/**
         * a vector of all nodes.  This makes selecting a node
         * at random an O(1) operation rather than O(N)
	 */
	std::vector<Node*> _node_vec;
	int _cons;
};

}

#endif
