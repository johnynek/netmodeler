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
bool compareNodes(const AddressedNode* a, const AddressedNode* b) {
    return a->getAddress() < b->getAddress();
}
*/
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
void DeetooNetwork::create(int n) {
    //Let's make n different nodes!!
    node_map.clear();
    while(node_map.size() < n) 
    //for(int j=0; j < n; j++) 
    {
	unsigned long int r_int_f = (unsigned long int)(_r_short.getDouble01() * (AMAX-1) );
	unsigned long int r_int_b = (unsigned long int)(_r_short.getDouble01() * (AMAX-1) );
	//cout << " r_int_f, r_int_b: " << r_int_f << ", " << r_int_b << endl;
	unsigned long int r_addr = r_int_f << 16 + r_int_b;
	//cout << "r_addr: " << r_addr << endl;
	std::set<std::string> items;
	items.clear();
	if (node_map.find(r_addr) == node_map.end() && r_addr != 0){
	    //cout << "addr: " << r_addr << endl;
            AddressedNode* anode = new AddressedNode(r_addr, items);
	    node_map[r_addr] = anode;
	    //cout << "nodemap.size() " << node_map.size() << endl;
	    add(anode);
	}
    }
    //cout << "how many node?: " << node_map.size() << endl;
			
    //Sort Nodes in address increasing order.
    //sort(node_vec.begin(), node_vec.end(), Compare_nodes);
    //sort(node_vec.begin(), node_vec.end() );
    
    //Form ring.
    //AddressedNode* first = itNode_map.begin()->second;
    AddressedNode* first = node_map.begin()->second;
    //node_vec.push_back(first);
    AddressedNode *tmp, *last = first;
    //_node_map[0] = first;
    add(first);
    std::map<unsigned long int, AddressedNode*>::iterator itNode_map;
    for (itNode_map = node_map.begin(); itNode_map != node_map.end(); itNode_map++) {
    //for(int k = 1; k < n; k++) 
        /**
	//cout << "----------------------------------------------------" << endl
		<< "\t query addr: \t" << (itNode_map->second)->getAddress(false) << endl
		<< "\t cache addr: \t" << (itNode_map->second)->getAddress(true) << endl
		<< "\t addr_i: \t" << (itNode_map->second)->addr_i << endl
		<< "\t addr_j: \t" << (itNode_map->second)->addr_j << endl;
	*/
        if (itNode_map == node_map.begin() ) {
		//do nothing. it is already added before for loop.
	}
	else
	{
          tmp = itNode_map->second;
	  if(!this->has(Edge(tmp,last) ) ) {
	  add(Edge(tmp, last));}
	  last = tmp;
	}
    }
    //Close the circle
    add(Edge(last, first));
    
    //Establishing shortcut connections:
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while( ni->moveNext() ) {
      AddressedNode* nodei = dynamic_cast<AddressedNode*> (ni->current());
      unsigned long int shortcut_address = nodei->getAddress(true);
      //cout << "current_addr: " << nodei->getAddress(true) << endl;
      while ( shortcut_address == nodei->getAddress(true) )
      {
        double x = _r_short.getDouble01();
        unsigned long int k = (unsigned long int) (pow((WMAX+1), x) );
        //cout << "k: " << k << endl;
	//cout << "node_i+k: " << (nodei->getAddress(true)+k) << endl;
        unsigned long int shortcut_target_addr = (nodei->getAddress(true) + k) % (WMAX );
	//cout << "+_+_+_+_+_+_+_+_++_+_+_+_+_+_+_+_+_+_+_+_+_+___+_+_+_++_+" << endl;
        //cout << "shortcut target addr: " << shortcut_target_addr << endl;
        shortcut_address = findShortcutAddress(shortcut_target_addr);
        AddressedNode* nodej = node_map[shortcut_address];
        //cout << "shortcut_addr from shortcut node: " << nodej->getAddress(true) << endl;
        //cout << " edge exist? " << getEdge(nodei, nodej) << endl;
        if ((nodei->getAddress(true) != nodej->getAddress(true)) && !(getEdge(nodei, nodej)) && !(getEdge(nodej, nodei))){
          //add(Edge(nodei,nodej));
	  //cout << "make short cut here====+++++++++++++++++++++++======" << endl;
          add( Edge(nodei,nodej) );
	}
      }
    }
    
}

void DeetooNetwork::createQueryNet(std::map<unsigned long int, AddressedNode*> nd_map)
{
    //sort(nd_vec.begin(), nd_vec.end() );
    //std::map<unsigned long int, AddressedNode*> query_nm;
    query_nm.clear();
    unsigned long int query_addr=0;
    AddressedNode* q_node;
    std::map<unsigned long int, AddressedNode*>::iterator itNd_map;
    for (itNd_map = nd_map.begin(); itNd_map != nd_map.end(); itNd_map++)
    {
	query_addr = (itNd_map->second)->getAddress(false);
	/**
	//cout << "----------------------------------------------------" << endl
		<< "\t query addr: \t" << query_addr << endl
		<< "\t cache addr: \t" << (itNd_map->second)->getAddress(true) << endl
		<< "\t addr_i: \t" << (itNd_map->second)->addr_i << endl
		<< "\t addr_j: \t" << (itNd_map->second)->addr_j << endl;
		*/
	q_node = itNd_map->second;
	query_nm[query_addr] = q_node;
    }
    
    AddressedNode* first = query_nm.begin()->second;
    add(first);
    AddressedNode *tmp, *last = first;
    for (itNd_map = query_nm.begin(); itNd_map != query_nm.end(); itNd_map++)
    //for (int k = 1; k<nd_vec.size(); k++)
    {
	//cout << " current node addr: " << itNd_map->first << endl;
        if (itNd_map == query_nm.begin() ) {
		//do nothing. it is already added before for loop.
	}
	else
	{
	  tmp = itNd_map->second;
	  add(tmp);
	  add(Edge(tmp, last));
	  last = tmp;
	}
    }
    add(Edge(last, first) );
    //cout << "how many edge? " << getEdgeSize() <<endl;
    //cout << "how many node? " << getNodeSize() << endl;
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while(ni->moveNext() ) {
	AddressedNode* nodei = dynamic_cast<AddressedNode*> (ni->current() );
	//cout << "****************************************************************" << endl;
	//cout << "current node: " << nodei->getAddress(false) << endl;
	unsigned long int shortcut_address = nodei->getAddress(false);
	while ( shortcut_address == nodei->getAddress(false) )
	{
	  double x = _r_short.getDouble01();
          unsigned long int k = (unsigned long int) pow( (WMAX+1), x);
          unsigned long int shortcut_target_addr = (nodei->getAddress(false) + k) % (WMAX );
          shortcut_address = findSCAQuery(shortcut_target_addr);
	  //cout << "shortcut Addr : " << shortcut_address << endl;
          AddressedNode* nodej = query_nm[shortcut_address];
          //cout << "+_+_+_+_+_+_+_+_++_+_+_+_+_+_+_+_+_+_+_+_+_+___+_+_+_++_+" << endl;
          if ((nodei->getAddress(false) != nodej->getAddress(false)) && !(getEdge(nodei, nodej)) && !(getEdge(nodej, nodei))){
            add( Edge(nodei,nodej) );
	  }
          //cout << "+_+_+_+_+_+_+_+_++_+_+_+_+_+_+_+_+_+_+_+_+_+___+_+_+_++_+" << endl;
      }
      //cout << "///////////////////////////////////////" << endl;
    }
    
}
/**
AddressedNode* DeetooNetwork::getNodeFromAddress(const int addr) const {
	map<int, AddressedNode*>::const_iterator i = _node_map.find(addr);
	if (i != _node_map.end()) {
		return i->second;
	}
	return 0;
}*/
// Find the shortcut node's address
// we have target address, let's find the nearest node 
// to the target address
unsigned long int DeetooNetwork::findShortcutAddress(unsigned long int t_addr) {
    unsigned long int this_distance, min_distance=WMAX, this_addr, scAddr;
    std::map<unsigned long int, AddressedNode*>::iterator itNM;
    //for (int ii=0; ii< node_map.size(); ii++) {
    for (itNM=node_map.begin(); itNM!=node_map.end(); itNM++) {
	//this_addr = node_map.at(ii)->_c_address;
	this_addr = itNM->first;
        this_distance = distanceTo( this_addr, t_addr);	    
	if ( this_distance < min_distance ) {
	    min_distance = this_distance;
	    scAddr = this_addr;
	}
    } 
    //cout << scAddr << endl;
    return scAddr;
}	
unsigned long int DeetooNetwork::findSCAQuery(unsigned long int t_addr) {
    unsigned long int this_distance, min_distance=WMAX, this_addr, scAddr;
    std::map<unsigned long int, AddressedNode*>::iterator itNM;
    //for (int ii=0; ii< node_map.size(); ii++) {
    for (itNM=query_nm.begin(); itNM!=query_nm.end(); itNM++) {
	//this_addr = node_map.at(ii)->_c_address;
	this_addr = itNM->first;
        this_distance = distanceTo( this_addr, t_addr);	    
	if ( this_distance < min_distance ) {
	    min_distance = this_distance;
	    scAddr = this_addr;
	}
    } 
    //cout << scAddr << endl;
    return scAddr;
}	
    
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

void DeetooNetwork::printNetInfo() {
   auto_ptr<NodeIterator> ni (this->getNodeIterator() );
   while ( ni->moveNext() ) {
      AddressedNode* this_node = dynamic_cast<AddressedNode*> ( ni-> current() );
      //cout<< "current node's address is: " << this_node->getAddress() << endl;
      //cout<< "current node's address is: " << ni->current()->getAddress() << endl;
      if (this->getDegree(this_node) != 0) {
         auto_ptr<NodeIterator> nni (this->getNeighborIterator(this_node) );
	 cout << "neighbors of " << this_node->getAddress(true) << "are: " << endl;
	 cout << "(";
	 while ( nni->moveNext() ) { 
	    AddressedNode* nei_node = dynamic_cast<AddressedNode*> (nni->current() );
	    cout << nei_node->getAddress(true) << ",";
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
