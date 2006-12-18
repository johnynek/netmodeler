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

#include "randroutingmessage.h"

using namespace Starsky;
using namespace std;

RandRoutingMessage::RandRoutingMessage(Ran1Random& r, double p_greedy, int nodes, int hops) : _r1ran(r), _p_greedy(p_greedy), _nodes(nodes),hops(0) {
}

void RandRoutingMessage::setTarget(RandAddrNode* t) {
        _target = t;
}
/*
void RandRoutingMessage::makeNewSWNetwork(SWNetwork* nnet) {
	new_net = nnet;
}
*/
SWNetwork* RandRoutingMessage::visit(Node* bstart, Network& net) 
{
	
   SWNetwork* new_net = dynamic_cast<SWNetwork*> (net.newNetwork() );
   //auto_ptr<SWNetwork> new_net ( dynamic_cast<SWNetwork*> (net.newNetwork() ) );
   RandAddrNode* start = dynamic_cast<RandAddrNode*> (bstart);
   cout << "first new_net's size: " << new_net->getNodeSize() << endl;
   new_net->add(start);
   cout << "after adding first node, new_net's size: " << new_net->getNodeSize() << endl;
   _start_addr = start->getAddress();
   _target_addr = _target->getAddress();
   r_send = _r1ran.getDouble01();
   cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
   //auto_ptr<Network> neighbor_net( net.getNeighbors(start) );
   //cout << "neighbor net's size: " << neighbor_net->getNodeSize() << endl;
   int init_distance = start->getDistanceTo(this->_nodes, _target);
   hops++;
   cout << "-------------------------hops---------------------" << hops << endl;
   if ((_start_addr!=_target_addr) && (hops < _nodes) && (net.getDegree(start) !=0)) 
   { 
      //select a node closest to a target with p_greedy
      if (_p_greedy > r_send)
      {
	 cout << "greedy mode" << endl;
         auto_ptr<NodeIterator> ni(net.getNeighborIterator(start));
         int min_distance = _nodes -1;
         while (ni->moveNext()) 
         {
            RandAddrNode* this_node = dynamic_cast<RandAddrNode*> (ni->current());
	    int this_distance = this_node->getDistanceTo(this->_nodes,_target);
	    if (this_distance < min_distance) 
	    {
	       min_distance = this_distance;
	       _greedy_next = this_node; 
	    }
         }
         if (min_distance <= init_distance) 
         {
            //go only forward.
	    //the distance to a target should be at least the same 
	    // as that of the previous node
	    new_net->add(this->_greedy_next);
            cout << "after adding greedy next node, new_net's size: " << new_net->getNodeSize() << endl;
	    //Edge new_edge = Edge(start, this->_greedy_next);
	    //new_net->add(new_edge);
	    cout << "current node and next node\t" << start->getAddress() << "\t" <<_greedy_next->getAddress() << endl;
	    new_net->add(Edge(start, _greedy_next) );
	    //RandRoutingMessage* m_greedy = new RandRoutingMessage(_r1ran, _p_greedy, _nodes, hops);
	    //auto_ptr<SWNetwork> visited_net (m_greedy->visit(_greedy_next, net) );
	    auto_ptr<SWNetwork> visited_net (visit(_greedy_next, net) );
	    new_net->add(visited_net.get() );
	    //delete m_greedy;
         }
      }	
      //With (1-p_greedy), select random node only among 'local neighbors' as next 
      else 
      {
	 cout << "random mode" << endl;
	 std::vector<SWEdge*> local_edges;
	 local_edges.clear();
	 auto_ptr<EdgeIterator> nei ( net.getEdgeIterator(start) );
	 while(nei->moveNext() ) {
	   SWEdge* c_edge (dynamic_cast<SWEdge*>(nei->current() ) );
	   if (c_edge->getAttributes() == "LC" ) {
	     local_edges.push_back(c_edge);
	   }
	 }
	 cout << "local_edges.size():\t" << local_edges.size() << endl;

         int ran_num=_r1ran.getInt(local_edges.size()-1); 	 
	 RandAddrNode* _rand_next = dynamic_cast<RandAddrNode*>(local_edges.at(ran_num)->getOtherNode(start) );
	 cout << "in random mode:\tcurrent node and next node:\t" << start->getAddress() << "\t" << _rand_next->getAddress() << endl;
	 new_net->add(_rand_next);
	 cout << "node added, new_net's size? \t" << new_net->getNodeSize() << endl;
	 new_net->add(Edge(start, _rand_next) );
	 //RandRoutingMessage* m_random = new RandRoutingMessage(_r1ran, _p_greedy, _nodes, hops);
	 //auto_ptr<SWNetwork> v_net (m_random->visit(_rand_next, net) );
	 auto_ptr<SWNetwork> v_net (visit(_rand_next, net) );
	 new_net->add(v_net.get() );
	 //visit(_rand_next, net);
	 //delete m_random;
      }
   }
   //SWNetwork* ret_val = new_net;
   //new_net = 0;
   cout << "ret_val's size: " << new_net->getNodeSize() << endl;
   cout << "ret_val have source?: " << new_net->has(start) << endl;
   //return ret_val;
   //return new_net.get();
   return new_net;
}
