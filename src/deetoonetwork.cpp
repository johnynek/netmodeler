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
DeetooNetwork::DeetooNetwork(int nodes, Ran1Random& r) : _r_short(r) {
    //node_vec.reserve(nodes);
    //create(nodes);
}
/**
bool DeetooNetwork::isIn(std::vector<AddressedNode*> n_vec, unsigned long int nd_addr) {
    std::vector<AddressedNode*>::iterator it;
    for (it = n_vec.begin(); it != n_vec.end(); it++) {
        if (nd_addr == (*it)->getCacheAddress() ) {
		return nd_addr;
	}
    }
}
*/
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
        //cout << "*****++++++++  short cut   +++++++++++++++++++++++++***" << endl
	//	<< "current node's address: " << nodei->getAddress(cache) << endl;
        double x = _r_short.getDouble01();
        unsigned long int k = (unsigned long int) (pow(10,(log10(WMAX)-(1-x)*log10(getNodeSize() ) ) ) );
	//cout << " shortcut distance(k) : " << k << endl;
        unsigned long int shortcut_target_addr = (nodei->getAddress(cache) + k) % (WMAX );
        //shortcut_address = findShortcutAddress(shortcut_target_addr);
        //AddressedNode* nodej = node_map[shortcut_address];
	AddressedNode* nodej = findShortcutNode(nd_map, shortcut_target_addr);
	shortcut_address = nodej->getAddress(cache);
	//cout << "selected shortcut neighbor's addr: "<< nodej->getAddress(cache) << endl;
        if ((nodei->getAddress(cache) != nodej->getAddress(cache)) && !(getEdge(nodei, nodej)) && !(getEdge(nodej, nodei))){
          add( Edge(nodei,nodej) );
	}
    }
  }
}

void DeetooNetwork::create(int n) {
    //Let's make n different nodes!!
    node_map.clear();
    //cout << "addresses : " ;
    while(node_map.size() < n) 
    {
	//unsigned long int r_int_f = (unsigned long int)(_r_short.getDouble01() * (AMAX-1) );
	//unsigned long int r_int_b = (unsigned long int)(_r_short.getDouble01() * (AMAX-1) );
	//unsigned long int r_addr = r_int_f << 16 + r_int_b;
	unsigned long int r_addr = (unsigned long int)(_r_short.getDouble01() * (WMAX) );
	//cout << r_addr << "\t";
	std::set<std::string> items;
	items.clear();
	if (node_map.find(r_addr) == node_map.end() && r_addr != 0){
            AddressedNode* anode = new AddressedNode(r_addr, items);
	    //cout << "node's cache addr: " << anode->getAddress(true) <<"\t" << "query address: " << anode->getAddress(false) << endl;
	    node_map[r_addr] = anode;
	    add(anode);
	}
    }
    //cout << endl << "-------------------Form Ring-------------------" << endl;			
    //Form ring.
    formRing(node_map);
    //cout << "edge size:\t" << getEdgeSize() << endl;    
    //cout << "-------------------Make Shortcut Connection-------------------" << endl;			
    //Establishing shortcut connections:
    makeShortcutConnection(node_map, true);
    //cout << "edge size:\t" << getEdgeSize() << endl;    
    //cout << "======================end of creation======================" << endl;
    cout << "==================cache network================" << endl;
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
	/*
	cout << "----------------------------------------------------" << endl
		<< "\t query addr: \t" << query_addr << endl
		<< "\t cache addr: \t" << (itNd_map->second)->getAddress(true) << endl
		<< "\t addr_i: \t" << (itNd_map->second)->addr_i << endl
		<< "\t addr_j: \t" << (itNd_map->second)->addr_j << endl;
	*/	
	q_node = dynamic_cast<AddressedNode*> (itNd_map->second);
	//cout << "q_node's address: " << q_node->getAddress(false) << endl;
	add(q_node);
	//cout << "after adding node size? " << getNodeSize() << endl;
	query_nm[query_addr] = q_node;
    }
    //cout << "-------------------Form Ring  for query-------------------" << endl;			
    //form ring.
    formRing(query_nm);
    //make shortcut connection.
    //cout << "-------------------Make Shortcut Connection  for query-------------------" << endl;			
    makeShortcutConnection(query_nm,false);
    cout << "======================end of creation for query network======================" << endl;
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
    //unsigned long int this_distance, min_distance=WMAX, this_addr, scAddr;
    std::map<unsigned long int, AddressedNode*>::iterator it_upper = n_map.lower_bound(t_addr);
    std::map<unsigned long int, AddressedNode*>::iterator it_lower = n_map.upper_bound(t_addr);
    if (distanceTo(it_upper->first,t_addr) < distanceTo(it_lower->first, t_addr) )
    {
      scNode = it_upper->second;
    }
    else 
    {
      scNode = it_lower->second;
    }
    /*
    for (itNM=n_map.begin(); itNM!=n_map.end(); itNM++) {
	this_addr = itNM->first;
        this_distance = distanceTo( this_addr, t_addr);	    
	if ( this_distance < min_distance ) {
	    min_distance = this_distance;
	    scAddr = this_addr;
	    scNode = n_map[scAddr];
	}
    } 
    */
    return scNode;
}	
/*
unsigned long int DeetooNetwork::findSCAQuery(unsigned long int t_addr) {
    unsigned long int this_distance, min_distance=WMAX, this_addr, scAddr;
    std::map<unsigned long int, AddressedNode*>::iterator itNM;
    for (itNM=query_nm.begin(); itNM!=query_nm.end(); itNM++) {
	//this_addr = node_map.at(ii)->_c_address;
	this_addr = itNM->first;
        this_distance = distanceTo( this_addr, t_addr);	    
	if ( this_distance < min_distance ) {
	    min_distance = this_distance;
	    scAddr = this_addr;
	}
    } 
    return scAddr;
}	
*/    
unsigned long int DeetooNetwork::distanceTo(unsigned long int addr_a, unsigned long int addr_b) {
    unsigned long int sm, bg, dt;
    //cout <<  "a and b: " << addr_a<< "\t"<< addr_b << endl;
    sm = std::min(addr_a, addr_b);
    bg = std::max(addr_a, addr_b);
    //cout << "small and big: " << sm << "\t" << bg << endl;
    dt = std::min( (bg-sm),( (WMAX+1) - bg + sm) );
    //cout << "distance:\t" << dt << endl;
    return dt;
}

void DeetooNetwork::printNetInfo(bool cache) {
   auto_ptr<NodeIterator> ni (getNodeIterator() );
   while ( ni->moveNext() ) {
      AddressedNode* this_node = dynamic_cast<AddressedNode*> ( ni-> current() );
      cout<< "current node's address is: " << this_node->getAddress(cache) << endl;
      //cout<< "current node's address is: " << ni->current()->getAddress() << endl;
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
/**
void DeetooNetwork::cacheItem(std::string content, AddressedNode* cn) 
{
    //Determine cache size
    int csize = (int) (sqrt( AMAX / this->getNodeSize() ) );
    unsigned long int start_addr;
    if ( cn->addr_i == 0) {cn->addr_i = 1;}
    start_addr = (unsigned long int)(cn->addr_i-2/csize)*AMAX;
    unsigned long int end_addr = (unsigned long int)( ( (start_addr / AMAX)+csize)*AMAX+AMAX-1);
    std::map<unsigned long int, AddressedNode*> cache_nm;
    std::map<unsigned long int, AddressedNode*>::iterator itNodeMap;
    for (itNodeMap=node_map.begin(); itNodeMap!=node_map.end(); itNodeMap++) 
    {
	std::set<std::string> cacheItemSet = itNodeMap->second->getItem();
	if ( (itNodeMap->first >= start_addr && itNodeMap->first <= end_addr) && cacheItemSet.find(content)== cacheItemSet.end() )
	{
	    itNodeMap->second->insertItem(content, cn);
	}
    }
}
*/
/**
DeetooNetwork* DeetooNetwork::queryForContent(AddressedNode* content, NodeIterator* ni)
{

}
*/
