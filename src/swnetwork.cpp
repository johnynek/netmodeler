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

#include "swnetwork.h"

using namespace Starsky;
using namespace std;

SWNetwork::SWNetwork(Ran1Random& r) : Network(), _r_short(r) {}
SWNetwork::SWNetwork(int nodes, Ran1Random& r, int local_size) : _r_short(r) {
    node_vec.reserve(nodes);
    //local_size = 2;
    create(nodes, local_size);
}

void SWNetwork::create(int n, int local_size) {
    RandAddrNode* first = new RandAddrNode(0);
    node_vec.push_back(first);
    RandAddrNode *tmp, *last = first;
    add(first);
    //We made the first node, now lets make the rest.
    for(int k = 1; k < n; k++) {
        tmp = new RandAddrNode(k);
	node_vec.push_back(tmp);
	add(tmp);
	add(SWEdge(tmp, last, "DN"));   //add 'direct neighbor' SWEdge to net
	//cout << "(" << tmp->getAddress() << "," << last->getAddress() << ")" ;
	last = tmp;
    }
    //Close the circle
    add(SWEdge(last, first, "DN"));

    /* test printing for edges
    cout << "(" << last->getAddress() << "," << first->getAddress() << ")" << endl;
    auto_ptr<EdgeIterator> ei1 (getEdgeIterator() );
    while (ei1->moveNext() ) {
      SWEdge* cedge (dynamic_cast<SWEdge*> ( ei1->current() ) );
      RandAddrNode* noi (dynamic_cast<RandAddrNode*> (ei1->current()->first) );
      RandAddrNode* noj (dynamic_cast<RandAddrNode*> (ei1->current()->second) );
      cout << "(" << noi->getAddress() << "," << noj->getAddress() << "," << cedge->printAttributes() << ")" ;
    }*/
    
    //This next part is making a shortcut connection:
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while( ni->moveNext() ) {
      RandAddrNode* nodei = dynamic_cast<RandAddrNode*> (ni->current());
      double x = _r_short.getDouble01();
      int k = int(pow(n, x));
      int shortcut_address = (nodei->getAddress() + k) % n;
      //cout << "c_addr and shortcut_addr: " << nodei->getAddress() << "," << shortcut_address << endl;
      RandAddrNode* nodej = node_vec[shortcut_address];
      //cout << "getEdge():\t" << (getEdge(nodei,nodej)==0)<<  endl;
      //cout << "nodei!= nodej?\t" << (nodei!=nodej) << endl;
      if ((nodei!=nodej) && (getEdge(nodei, nodej)==0)){
	  //cout << getEdge(nodei,nodej) << endl;
          add(SWEdge(nodei,nodej, "SC"));
	  //cout << "-----------------------------------------" << endl;
	  //cout << "(" << nodei->getAddress() << "," << nodej->getAddress() << ")" << endl;
      }
    }
/*
    //this is for testing EdgeIterator
    auto_ptr<EdgeIterator> ei (getEdgeIterator() );
    while (ei->moveNext() ) {
      //cout << (ei->current() )->toString().c_str()<< endl;
      SWEdge* cedge (dynamic_cast<SWEdge*> ( ei->current() ) );
      if (cedge==0) {
	      cout << "make swedge properly!!!!" << endl;
      }
      RandAddrNode* noi (dynamic_cast<RandAddrNode*> (ei->current()->first) );
      RandAddrNode* noj (dynamic_cast<RandAddrNode*> (ei->current()->second) );
      if (noi==0 || noj==0) {
        cout << "make addressed  node properly!!!!!" << endl;
      }
      else {
        cout << "(" << noi->getAddress() << "," << noj->getAddress() << "," << cedge->printAttributes() << ")" ;
      }
    }// delete c_edge;
*/
    //Now we make "local connections" which are a distance log N or less
    int log_dist = (int)(log10(n)/log10(2) );
    auto_ptr<NodeIterator> nl(getNodeIterator() ) ;
    while (nl->moveNext() ) {
      RandAddrNode* c_node = dynamic_cast<RandAddrNode*>(nl->current() );
      std::vector<RandAddrNode*> local_neis;
      auto_ptr<NodeIterator> nei(getNodeIterator() );
      bool everylocal=false;
      while(nei->moveNext() ) {
	RandAddrNode* nnode = dynamic_cast<RandAddrNode*>(nei->current() );
	int cur_dist = c_node->getDistanceTo(n,nnode);
	//cout << "\t cur_dist and log_dist:\t" << cur_dist << "\t" << log_dist << endl;
	if (cur_dist <= log_dist && c_node!=nnode)
	{
	  if (getEdge(c_node, nnode)==0)
	  {
	    //cout << "c_node and selected node addr:\t" << c_node->getAddress() << "\t" << nnode->getAddress() << endl;
	    c_node->local_neighbors.push_back(nnode);
	    local_neis.push_back(nnode);
	    if (local_size == log_dist*2)  //if local nei's size ==logN, add every node in the range.
	    {
	      everylocal = true;
	      //cout << "---------add SWEdge here!!!!!!!!!!!!!!!----------------" << endl;
              add(SWEdge(c_node, nnode, "LC") );
	    }
	  }
	}
      }
      //cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
      //cout << "current Addr:\t " << c_node->getAddress() << endl;
      if (!everylocal) {
        int local_addr;
	int le_cnt=0;
        RandAddrNode* ln_node;
	std::set<RandAddrNode*> ln_nodes;
	//std::set<int> ran_nums;
	//cout << "local_neis.size: " << local_neis.size() << endl; 
        do {
	  local_addr = _r_short.getInt(local_neis.size()-1);
	  ln_node = local_neis.at(local_addr);
	  //cout <<" selected local neighbor node:\t" << ln_node->getAddress() << endl;
	  if (ln_nodes.find(ln_node) == ln_nodes.end() ) {
	    ln_nodes.insert(ln_node);
	  //if (ran_nums.find(local_addr) == ran_nums.end() ) {
	  //  ran_nums.insert(local_addr);
	  }
        }
        //while ( ran_nums.size()<(local_size) );
        while ( ln_nodes.size()<(local_size) );
	//cout << "\t\tprint how many random numbers:\t " << ln_nodes.size() << endl;
	std::set<RandAddrNode*>::iterator nodeit;
	for (nodeit = ln_nodes.begin(); nodeit!=ln_nodes.end(); nodeit++) {
	  le_cnt=le_cnt+1;
	  add(SWEdge(c_node, *nodeit, "LC") );
	}
        //cout << "how many local edges for " << c_node->getAddress() << " node? " << le_cnt << endl;
      }
    }
/*
    int lc_cnt = 0;
    auto_ptr<EdgeIterator> ei2 (getEdgeIterator() );
    while (ei2->moveNext() ) {
      //cout << (ei1->current() )->toString().c_str()<< endl;
      SWEdge* cedge (dynamic_cast<SWEdge*> ( ei2->current() ) );
      RandAddrNode* noi (dynamic_cast<RandAddrNode*> (ei2->current()->first) );
      RandAddrNode* noj (dynamic_cast<RandAddrNode*> (ei2->current()->second) );
      cout << "(" << noi->getAddress() << "," << noj->getAddress() << "," << cedge->printAttributes() << ")" ;
      if (cedge->getAttributes()=="LC") { lc_cnt=lc_cnt+1; }
    }
    cout << "total number of LC is: \t" << lc_cnt << endl;
    */
}

SWNetwork* SWNetwork::getLocalNeighbors(RandAddrNode* node) const {
   auto_ptr<SWNetwork> all_nei (dynamic_cast<SWNetwork*> (getNeighbors(node) ) );
   auto_ptr<NodeIterator> ni (all_nei->getNodeIterator() );
   int log_dist = (int)(log10(getNodeSize() )/log10(2) );
   //auto_ptr<SWNetwork> net (new SWNetwork(_r_short) );
   SWNetwork* net = new SWNetwork(_r_short) ;
   while (ni->moveNext() )
   {
      RandAddrNode* c_node = dynamic_cast<RandAddrNode*> (ni->current() ) ;
      //cout << "start node in getLocalNeighbors() " << node->getAddress() << endl;
      //cout << "current node in getLocalNeighbors() " << c_node->getAddress() << endl;
      if (c_node->getDistanceTo(getNodeSize(),node) <= log_dist)
      {
	      cout <<  "Adding node++" << endl;

	 net->add(c_node );
	 //delete c_node;
      }
      else {
      }
   }
   cout << "new_net's size " << net->getNodeSize() << endl;
   
   auto_ptr<NodeIterator> nii (net->getNodeIterator() );
   while ( nii->moveNext() ) {
	   auto_ptr<RandAddrNode> th_node ( dynamic_cast<RandAddrNode*> (nii->current() ) );
	   cout << th_node->getAddress() << endl;
   }
   //SWNetwork* ret_net = net.get();
   return net;
   /*
   map<RandAddrNode*, EdgeSet>::const_iterator it;
   it = _node_to_edges.find(node);
   SWNetwork* net = newSWNetwork();
   int log_dist = (int)(log10(_nodes)/log10(2) );
   if(it != _node_to_edges.end() ) {
      EdgeSet::const_iterator eit;
      for(eit = it->second.begin(); eit != it->second.end(); eit++) {
	  auto_ptr<RandAddrNode*> t_node ( (*eit)->getOtherNode(node) );
	  if ( t_node->getDistanceTo(_node, node) <= log_dist) {
	     net->add( t_node);
	  }
      }
   }
   else {}
   return net;
   */
}
	
void SWNetwork::printNetInfo() {
   auto_ptr<NodeIterator> ni (this->getNodeIterator() );
   while ( ni->moveNext() ) {
      RandAddrNode* this_node = dynamic_cast<RandAddrNode*> ( ni-> current() );
      //cout<< "current node's address is: " << this_node->getAddress() << endl;
      //cout<< "current node's address is: " << ni->current()->getAddress() << endl;
      if (this->getDegree(this_node) != 0) {
         auto_ptr<NodeIterator> nni (this->getNeighborIterator(this_node) );
	 cout << "neighbors of " << this_node->getAddress() << "are: " << endl;
	 cout << "(";
	 while ( nni->moveNext() ) { 
	    RandAddrNode* nei_node = dynamic_cast<RandAddrNode*> (nni->current() );
	    cout << nei_node->getAddress() << ",";
         }
	 cout << ")" << endl;
      }
      else {
         cout << "no neighbor" << endl;
      }
   }
}
	
void SWNetwork::printNeighborEdgeInfo() {
   auto_ptr<NodeIterator> ni (this->getNodeIterator() );
   while ( ni->moveNext() ) {
      RandAddrNode* this_node = dynamic_cast<RandAddrNode*> ( ni-> current() );
      if (this->getDegree(this_node) != 0) {
	 cout << "edges of " << this_node->getAddress() << " are: " << endl;
	 cout << "(";
	 auto_ptr<EdgeIterator> ei ( getEdgeIterator(ni->current() ) );
	 while( ei->moveNext() ) {
	   SWEdge* c_edge (dynamic_cast<SWEdge*> (ei->current() ) );
	   RandAddrNode* fnode ( dynamic_cast<RandAddrNode*> (c_edge->first) );
	   RandAddrNode* snode ( dynamic_cast<RandAddrNode*> (c_edge->second) );
	   cout << "[" << fnode->getAddress() << "," << snode->getAddress() << "," << c_edge->printAttributes() << "]" ;
         }
	 cout << ")" << endl;
      }
      else {
         cout << "no neighbor" << endl;
      }
   }
}
		   	
		   	
vector<int> SWNetwork::getNeighborDistHist(int bins) const {

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
	 RandAddrNode* node_first = dynamic_cast<RandAddrNode*> (e->first);
	 RandAddrNode* node_second = dynamic_cast<RandAddrNode*> (e->second) ;
         this_nd =  node_first->getDistanceTo(this->getNodeSize(), node_second) ;
         this_bin = this_nd * bins;
	 ret_val[this_bin] = ret_val[this_bin] + 1;
      //}
   }
   return ret_val;
}
