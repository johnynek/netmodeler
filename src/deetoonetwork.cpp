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

#include "deetoonetwork.h"
#include <cmath>

using namespace Starsky;
using namespace std;

DeetooNetwork::DeetooNetwork(Ran1Random& r) : Network(), _r_short(r) {}
DeetooNetwork::DeetooNetwork(int nodes, Ran1Random& r) : _r_short(r) {}

#define AMAX 65536
#define WMAX 4294967295
void DeetooNetwork::formRing(std::map<unsigned long int, AddressedNode*> nodeMap) {
  AddressedNode* first = nodeMap.begin()->second;
  AddressedNode *tmp, *last = first;
  add(first);
  std::map<unsigned long int, AddressedNode*>::iterator itNodeMap;
  for (itNodeMap = nodeMap.begin(); itNodeMap != nodeMap.end(); itNodeMap++) {
    if (itNodeMap == nodeMap.begin() ) {
      //do nothing. it is already added before for loop.
    }
    else {
      tmp = itNodeMap->second;
      if (!this->has(Edge(tmp, last) ) ) {
        add(Edge(tmp, last) );
      }
      last = tmp;
    }
  }
  add(Edge(last,first) );
}

void DeetooNetwork::makeShortcutConnection(std::map<unsigned long int, AddressedNode*> nd_map, bool cache) {
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  while(ni->moveNext() ) {
    AddressedNode* nodei = dynamic_cast<AddressedNode*> (ni->current() );
    unsigned long int shortcut_address = nodei->getAddress(cache);
    while (shortcut_address == nodei->getAddress(cache) ) {
        double x = _r_short.getDouble01();
        unsigned long int k = (unsigned long int) (pow(10,(log10(WMAX)-(1-x)*log10(getNodeSize() ) ) ) );
        unsigned long int shortcut_target_addr = (nodei->getAddress(cache) + k) % (WMAX );
	AddressedNode* nodej = findShortcutNode(nd_map, shortcut_target_addr);
	shortcut_address = nodej->getAddress(cache);
        if ((nodei->getAddress(cache) != nodej->getAddress(cache)) && !(getEdge(nodei, nodej)) && !(getEdge(nodej, nodei))){
          add( Edge(nodei,nodej) );
	}
    }
  }
}

void DeetooNetwork::create(int n) {
    //Let's make n different nodes!!
    node_map.clear();
    while(node_map.size() < n) 
    {
	unsigned long int r_addr = (unsigned long int)(_r_short.getDouble01() * (WMAX) );
	std::set<std::string> items;
	items.clear();
	if (node_map.find(r_addr) == node_map.end() && r_addr != 0){
            AddressedNode* anode = new AddressedNode(r_addr, items);
	    node_map[r_addr] = anode;
	    add(anode);
	}
    }
    //Form ring.
    formRing(node_map);
    //Establishing shortcut connections:
    makeShortcutConnection(node_map, true);
    //printNetInfo(true);
}

void DeetooNetwork::createQueryNet(std::map<unsigned long int, AddressedNode*> nd_map)
{
    //using addresses in query space, make node map for query.
    query_nm.clear();
    unsigned long int query_addr=0;
    AddressedNode* q_node;
    std::map<unsigned long int, AddressedNode*>::iterator itNd_map;
    for (itNd_map = nd_map.begin(); itNd_map != nd_map.end(); itNd_map++)
    {
	query_addr = (itNd_map->second)->getAddress(false);
	
	//cout << "----------------------------------------------------" << endl
        //	<< "\t query addr: \t" << query_addr << endl
	//	<< "\t cache addr: \t" << (itNd_map->second)->getAddress(true) << endl;
		
	q_node = dynamic_cast<AddressedNode*> (itNd_map->second);
	add(q_node);
	query_nm[query_addr] = q_node;
    }
    
    //form ring.
    formRing(query_nm);
    //make shortcut connection.
    makeShortcutConnection(query_nm,false);
    //printNetInfo(false);
    
}
/**
AddressedNode* DeetooNetwork::getNodeFromAddress(const int addr) const {
	map<int, AddressedNode*>::const_iterator i = _node_map.find(addr);
	if (i != _node_map.end()) {
		return i->second;
	}
	return 0;
}*/

// Find the shortcut node
// we have target address, let's find the nearest node 
// to the target address
AddressedNode* DeetooNetwork::findShortcutNode(std::map<unsigned long int, AddressedNode*> n_map, unsigned long int t_addr) {
    AddressedNode* scNode;
    std::map<unsigned long int, AddressedNode*>::iterator it_upper = n_map.upper_bound(t_addr);
    std::map<unsigned long int, AddressedNode*>::iterator it_upper1 = it_upper;
    it_upper1--;
    if (it_upper == n_map.begin() || it_upper == n_map.end() )
    {
      if (distanceTo( (n_map.begin() )->first, t_addr) < distanceTo( (n_map.rbegin() )->first, t_addr) ) {
        return (n_map.begin() )->second;
      }
      else {
        return (n_map.rbegin() )->second;
      }
    }
    else {
      if (distanceTo(it_upper->first, t_addr) < distanceTo( it_upper1->first, t_addr) ) {
        return it_upper->second;
      }
      else {
        return it_upper1->second;
      }
    }
}	

unsigned long int DeetooNetwork::distanceTo(unsigned long int addr_a, unsigned long int addr_b) {
    unsigned long int sm, bg, dt;
    sm = std::min(addr_a, addr_b);
    bg = std::max(addr_a, addr_b);
    dt = std::min( (bg-sm),( (WMAX+1) - bg + sm) );
    return dt;
}

void DeetooNetwork::printNetInfo(bool cache) {
   auto_ptr<NodeIterator> ni (getNodeIterator() );
   while ( ni->moveNext() ) {
      AddressedNode* this_node = dynamic_cast<AddressedNode*> ( ni-> current() );
      cout<< "current node's address is: " << this_node->getAddress(cache) << endl;
      if (this->getDegree(this_node) != 0) {
         auto_ptr<NodeIterator> nni (this->getNeighborIterator(this_node) );
	 cout << "neighbors of " << this_node->getAddress(cache) << "are: " << endl;
	 cout << "(";
	 while ( nni->moveNext() ) { 
	    AddressedNode* nei_node = dynamic_cast<AddressedNode*> (nni->current() );
	    cout << nei_node->getAddress(cache) << ",";
         }
	 cout << ")" << endl;
      }
      else {
         cout << "no neighbor" << endl;
      }
   }
}
		   	
vector<int> DeetooNetwork::getNeighborDistHist(int bins) const {

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

std::pair<unsigned long int, unsigned long int> DeetooNetwork::getRange(double cq_size) {
  unsigned long int range0, range1;
  //cout << "set ranges==================" << endl;
  int start_cr = (int)(_r_short.getDouble01()*(double)(AMAX - cq_size - 1 ) );
  double end_cr = (double)start_cr + cq_size-1;
 if (end_cr > (AMAX-1) ) {
     range1 = WMAX;
     range0 = (unsigned long int)( ( (AMAX-1) - cq_size +1 ) * AMAX);
 } 
 else {
     range0 = start_cr * AMAX;
     range1 = (unsigned long int) ( (end_cr * AMAX) + AMAX -1 );
 }
 //cout << "range0: " << range0 << ", range1: " << range1 << endl; 
  return make_pair(range0, range1);
}
