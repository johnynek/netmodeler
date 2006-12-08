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

#ifndef starsky__RandRoutingmessage
#define starsky__RandRoutingmessage

#include "message.h"
#include "randaddrnode.h"
#include "swnetwork.h"
#include "random.h"
#include "ran1random.h"
#include "uniformnodeselector.h"
#include <set>
#include <map>
#include <vector>

namespace Starsky {

 /**
  * Message in Small World Network which is sent by Greedy (with P_greedy) 
  *        or Randomized Routing with some probability(P_rand = 1-P_greedy).
  * It sets a destination node.
  * Also it returns subnetwork which include every nodes and edges visited.
  */
class RandRoutingMessage : public Message {

    public:
	/**
	 * @param r random number between [0.0, 1.0]
	 * @param p_greedy select greedy routing algorithm with probability of 
	 * @param nodes total number of nodes of a network
	 * @param hops count number of hops visited
	 */
        RandRoutingMessage(Ran1Random& r, double p_greedy, int nodes, int hops);
	/**
	 * @param target created SWNetwrok type network 
	 * returns randomly selected target node in my_net
	 */
	void setTarget(RandAddrNode* target);
	/**
	 * @param start starting node from which message transfers
	 * @param net current network
	 * returns subnetwork of net consisting of nodes and edges 
	 */
	SWNetwork* visit(Node* start, Network& net);
	/**
	 * @param nnet new SWNetwork
	 * make new empty SWNetwork
	 */
	//void makeNewSWNetwork(SWNetwork* nnet); 
	int min_distance;
	//SWNetwork* new_net;
	double r_send;
	int hops;

    protected:
	RandAddrNode* _target;
	int _start_addr;
	int _target_addr;
	Ran1Random& _r1ran;
	RandAddrNode* _greedy_next;
	RandAddrNode* _rand_next;
	double _p_greedy;
	int _nodes;
};

}

#endif
