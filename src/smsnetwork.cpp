/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
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

#include "smsnetwork.h"
#include <cmath>

using namespace Starsky;
using namespace std;

Smsnetwork::Smsnetwork(Ran1Random& r) : Network(), _r_short(r) {}
Smsnetwork::Smsnetwork(int nodes, Ran1Random& r) : _r_short(r) {
    node_vec.reserve(nodes);
    create(nodes);
}
/**
bool compareNodes(const AddressedNode* a, const AddressedNode* b) {
    return a->getAddress() < b->getAddress();
}
*/
bool isIn(std::vector<AddressedNode*> n_vec, unsigned long int nd_addr) {
    std::vector<AddressedNode*>::iterator it;
    for (it = n_vec.begin(); it != n_vec.end(); it++) {
        if (nd_addr == (*it)->getCacheAddress() ) {
		return nd_addr;
	}
    }
}

#define AMAX 65536
void Smsnetwork::create(int n) {
    for(int j=0; j < n; j++) {
	unsigned long int r_int = (unsigned long int)_r_short.getDouble01() * AMAX;
	r_addr = r_int << 16 + r_int;
	//r_addr = r_int.getInt( (AMAX-1), 0 ) << 16 + r_int.getInt( (AMAX-1), 0 );
	//while(_node_map.find(r_addr) !=_node_map.end() ){
	while( !isIn(node_vec, r_addr));
            AddressedNode* anode = new AddressedNode(r_addr);
	    //node_map[r_addr] = anode;
            node_vec.push_back(anode);
	    add(anode);
	}
	//while( !isIn(node_vec, anode));
    }
			
    //Sort Nodes in address increasing order.
    //sort(node_vec.begin(), node_vec.end(), Compare_nodes);
    sort(node_vec.begin(), node_vec.end() );
    
    AddressedNode* first = node_vec[0];
    //node_vec.push_back(first);
    AddressedNode *tmp, *last = first;
    //_node_map[0] = first;
    add(first);
    //Form ring.
    //std::vector<AddressedNode*>::iterator itNode_vec;
    //for (itNode_vec = node_vec.begin(); itNode_vec != node_vec.end(); itNode_vec++) {
    for(int k = 1; k < n; k++) 
        tmp = node_vec[k];
        //tmp = *(itNode_vec);
	//add(tmp);
	//_node_map[k] = tmp;
	add(Edge(tmp, last));
	last = tmp;
    }
    //Close the circle
    add(Edge(last, first));
    
    //Establishing shortcut connections:
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while( ni->moveNext() ) {
      AddressedNode* nodei = dynamic_cast<AddressedNode*> (ni->current());
      double x = _r_short.getDouble01();
      unsigned long int k = unsigned long int(pow(AMAX, x));
      unsigned long int shortcut_target_addr = (nodei->getCacheAddress() + k) % AMAX;
      unsigned long int shortcut_address = findShortcutAddress(shortcut_target_addr);
      AddressedNode* nodej = node_vec[shortcut_address];
      if ((nodei->getAddress() != nodej->getAddress()) && !(getEdge(nodei, nodej)) && !(getEdge(nodej, nodei))){
          //add(Edge(nodei,nodej));
          add( Edge(nodei,nodej) );
      }

    }
    
}
/**
AddressedNode* Smsnetwork::getNodeFromAddress(const int addr) const {
	map<int, AddressedNode*>::const_iterator i = _node_map.find(addr);
	if (i != _node_map.end()) {
		return i->second;
	}
	return 0;
}*/


// Find the shortcut node's address
// we have target address, let's find the nearest node 
// to the target address
unsigned long int findShortcutAddress(unsigned long int t_addr) {
    unsigned long int this_distance, min_distance=AMAX, this_addr, scAddr;
    for (ii=0, ii< node_vec.size(), ii++) {
	this_addr = node_vec.at(ii)->_c_address;
        this_distance = distnceTo( this_addr, t_addr);	    
	if ( this_distance < min_distance) {
	    min_distance = this_distance;
	    scAddr = this_addr;
	}
    } return scAddr;
}	
    
unsigned long int distanceTo(unsigned long int addr_a, unsigned long int addr_b) {
    unsigned long int sm, bg, dt;
    sm = std::min(addr_a, addr_b);
    bg = std::max(addr_a, addr_b);
    return dt = std::min( (bg-sm),(AMAX-bg+sm) );
}

void Smsnetwork::printNetInfo() {
   auto_ptr<NodeIterator> ni (this->getNodeIterator() );
   while ( ni->moveNext() ) {
      AddressedNode* this_node = dynamic_cast<AddressedNode*> ( ni-> current() );
      //cout<< "current node's address is: " << this_node->getAddress() << endl;
      //cout<< "current node's address is: " << ni->current()->getAddress() << endl;
      if (this->getDegree(this_node) != 0) {
         auto_ptr<NodeIterator> nni (this->getNeighborIterator(this_node) );
	 cout << "neighbors of " << this_node->getAddress() << "are: " << endl;
	 cout << "(";
	 while ( nni->moveNext() ) { 
	    AddressedNode* nei_node = dynamic_cast<AddressedNode*> (nni->current() );
	    cout << nei_node->getAddress() << ",";
         }
	 cout << ")" << endl;
      }
      else {
         cout << "no neighbor" << endl;
      }
   }
}
		   	
vector<int> Smsnetwork::getNeighborDistHist(int bins) const {

   vector<int> ret_val;
   int this_nd;
   int this_bin;

   ret_val.resize(bins);
   for (int j = 0; j < bins; j++) {
      ret_val[j] = 0;
   }
   auto_ptr<EdgeIterator> ei( getEdgeIterator() );
   while(ei->moveNext() ) {
      Edge* e = ei->current();
      //if (e->getAttributes() == "SC") {
	 AddressedNode* node_first = dynamic_cast<AddressedNode*> (e->first);
	 AddressedNode* node_second = dynamic_cast<AddressedNode*> (e->second) ;
         this_nd =  node_first->getDistanceTo(this->getNodeSize(), node_second) ;
         this_bin = this_nd * bins;
	 ret_val[this_bin] = ret_val[this_bin] + 1;
      //}
   }
   return ret_val;
}
