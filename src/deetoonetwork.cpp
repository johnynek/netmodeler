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
void DeetooNetwork::formRing(const std::map<unsigned long int, AddressedNode*>& nodeMap) {
  AddressedNode* first = nodeMap.begin()->second;
  AddressedNode *tmp, *last = first;
  add(first);
  std::map<unsigned long int, AddressedNode*>::const_iterator itNodeMap;
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

void DeetooNetwork::makeShortcutConnection(const std::map<unsigned long int, AddressedNode*>& nd_map, bool cache) {
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  while(ni->moveNext() ) {
    AddressedNode* nodei = dynamic_cast<AddressedNode*> (ni->current() );
    unsigned long int shortcut_address = nodei->getAddress(cache);
    while (shortcut_address == nodei->getAddress(cache) ) {
        double x = _r_short.getDouble01();
	int net_size = guessNetSize(nodei, cache);
	//int net_size = getNodeSize();
        unsigned long int k = (unsigned long int) (pow(10,(log10(WMAX)-(1-x)*log10(net_size ) ) ) );
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

void DeetooNetwork::createQueryNet(const std::map<unsigned long int, AddressedNode*>& nd_map)
{
    //using addresses in query space, make node map for query.
    query_nm.clear();
    unsigned long int query_addr=0;
    AddressedNode* q_node;
    std::map<unsigned long int, AddressedNode*>::const_iterator itNd_map;
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
AddressedNode* DeetooNetwork::findShortcutNode(const std::map<unsigned long int, AddressedNode*>& n_map, unsigned long int t_addr) {
    AddressedNode* scNode;
    std::map<unsigned long int, AddressedNode*>::const_iterator it_upper = n_map.upper_bound(t_addr);
    std::map<unsigned long int, AddressedNode*>::const_iterator it_upper1 = it_upper;
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

int DeetooNetwork::guessNetSize(AddressedNode* tnode,bool cq)
{
  std::map<int,AddressedNode*> distances;
  distances.clear();
  
  //int count = 0;
  unsigned long int addr_min = WMAX;
  unsigned long int addr_max = 0;
  int this_dist, dist1, dist2;
  unsigned long int this_addr;
  auto_ptr<NodeIterator> ni(getNeighborIterator(tnode) );
  while (ni->moveNext() ) {
    AddressedNode* c_node = dynamic_cast<AddressedNode*> (ni->current() );
    this_dist = c_node->getDistanceTo(tnode->getAddress(cq),cq);
    this_addr = c_node->getAddress(cq);
    //cout << "this_dist: " << this_dist << endl;
    distances[this_dist]=c_node;
    if (this_addr < addr_min) { addr_min = this_addr; }
    if (this_addr > addr_max) { addr_max = this_addr; }
  }
  if (tnode->getAddress(cq) < addr_min || tnode->getAddress(cq) > addr_max ) {
    dist1 = tnode->getDistanceTo(addr_min, cq);
    dist2 = tnode->getDistanceTo(addr_max, cq);
  }
  else {
    std::map<int,AddressedNode*>::iterator dist_it = distances.begin();
    dist1 = dist_it->first;
    dist_it++;
    dist2 = dist_it->first;
  }
  //cout << "dist1 and dist2 : " << dist1 << "\t" << dist2 << endl;
  unsigned long int d_ave = (unsigned long int) ( (dist1 + dist2) / 2);
  //cout << "d_ave: " << d_ave << endl;
  int d_net = (int)( (WMAX / d_ave) + (1 / d_ave) );
  //cout << "d_net: " << d_net << endl;
  /** 
  int log_d = (int)(log(d_net) );
  cout << "--------------------------------" << endl;
  std::map<unsigned long int, AddressedNode*>::const_iterator upper = node_map.upper_bound(tnode->getAddress(cq) );
  for (int iter = 0; iter < log_d; iter++) {
    if (upper == node_map.end() ) {
      upper = node_map.begin();
    }
    cout << upper->first << endl;
    upper++;
  }
  int dist_to = tnode->getDistanceTo(upper->first, cq);
  cout << dist_to << "\t" << log_d << endl;
  double temp = (double)(log_d) / (double)(dist_to);
  //cout << "temp: " << temp << endl;
  int new_est = (int)( temp * WMAX);
  //cout << new_est << endl;
  return new_est;
  **/
  return d_net;
}
    
void DeetooNetwork::createEvenNet(int net_size) {
  node_map.clear();
  std::set<std::string> items;
  items.clear();
  //add first 2 nodes in the network.
  while(node_map.size() < 2) 
  {
    unsigned long int r_addr = (unsigned long int)(_r_short.getDouble01() * (WMAX) );
    if (node_map.find(r_addr) == node_map.end() && r_addr != 0){
      AddressedNode* anode = new AddressedNode(r_addr, items);
      //cout << "first address: " << r_addr << endl;
      node_map[r_addr] = anode;
      add(anode);
    }
  }
  //cout << "node_map.size(): " << node_map.size() << endl;
  //add all the others.
  unsigned long int r_addr1, r_addr2, up_addr, down_addr;
  int dist2up, dist2down;
  int dist1, dist2;
  std::map<unsigned long int, AddressedNode*>::const_iterator it_up;
  while( node_map.size() < net_size) {
    //cout << "------------------------------" << endl;
    //cout << "node_map.size(): " << node_map.size() << endl;
    //first location
    r_addr1 = (unsigned long int)(_r_short.getDouble01() * (WMAX) );
    //cout << "addr1: " << r_addr1 << endl;
    it_up = node_map.upper_bound(r_addr1 );
    up_addr =  it_up->first;
    down_addr = (it_up--)->first;
    dist2up = distanceTo(up_addr, r_addr1); 
    dist2down = distanceTo(down_addr, r_addr1);
    //dist1 = dist2up + dist2down; 
    dist1 = std::min(dist2up , dist2down); 
    //second location
    r_addr2 = (unsigned long int)(_r_short.getDouble01() * (WMAX) );
    //cout << "addr2: " << r_addr2 << endl;
    it_up = node_map.upper_bound(r_addr2 );
    up_addr =  it_up->first;
    down_addr = (it_up--)->first;
    dist2up = distanceTo(up_addr, r_addr2); 
    dist2down = distanceTo(down_addr, r_addr2); 
    //dist2 = dist2up + dist2down; 
    dist2 = std::min(dist2up , dist2down); 
    //choose bigger distance
    AddressedNode* new_node;
    unsigned long int new_addr;
    if (dist1 >= dist2) {
      new_node = new AddressedNode(r_addr1,items);
      new_addr = r_addr1;
    }
    else {
      new_node = new AddressedNode(r_addr2,items);
      new_addr = r_addr2;
    }
    //cout << "new addr: " << new_addr << endl;
    if (node_map.find(new_addr) == node_map.end() && new_addr != 0) {
      node_map[new_addr] = new_node;	    
      add(new_node);
    }
  }
  formRing(node_map);
  makeShortcutConnection(node_map, true);
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
 if (end_cr >= (AMAX-1) ) {
     range1 = WMAX;
     range0 = (unsigned long int)( ( (AMAX-1) - cq_size +1 ) * AMAX);
 } 
 else {
     range0 = start_cr * AMAX;
     range1 = (unsigned long int) ( (end_cr * AMAX) + AMAX -1 );
 }
  return make_pair(range0, range1);
}
