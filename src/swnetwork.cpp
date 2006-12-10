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
    //SWEdge* c_edge;
    //_node_map[0] = first;
    add(first);
    //We made the first node, now lets make the rest.
    for(int k = 1; k < n; k++) {
        tmp = new RandAddrNode(k);
	node_vec.push_back(tmp);
	add(tmp);
	//_node_map[k] = tmp;
	//c_edge = new SWEdge(tmp, last, "DN");
	//add(*c_edge);
	add(SWEdge(tmp, last, "DN"));   //add 'direct neighbor' SWEdge to net
	cout << "(" << tmp->getAddress() << "," << last->getAddress() << ")" ;
	last = tmp;
    }
    //Close the circle
    //c_edge = new SWEdge(last, first, "DN");
    add(SWEdge(last, first, "DN"));
    cout << "(" << last->getAddress() << "," << first->getAddress() << ")" << endl;
    //add(*c_edge);
    auto_ptr<EdgeIterator> ei1 (getEdgeIterator() );
    while (ei1->moveNext() ) {
      //cout << (ei1->current() )->toString().c_str()<< endl;
      SWEdge* cedge (dynamic_cast<SWEdge*> ( ei1->current() ) );
      RandAddrNode* noi (dynamic_cast<RandAddrNode*> (ei1->current()->first) );
      RandAddrNode* noj (dynamic_cast<RandAddrNode*> (ei1->current()->second) );
      cout << "(" << noi->getAddress() << "," << noj->getAddress() << "," << cedge->printAttributes() << ")" ;
    }
    cout << "How manay edges? " << getEdgeSize() << endl;
    
    //This next part is making a shortcut connection:
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while( ni->moveNext() ) {
      RandAddrNode* nodei = dynamic_cast<RandAddrNode*> (ni->current());
      double x = _r_short.getDouble01();
      int k = int(pow(n, x));
      int shortcut_address = (nodei->getAddress() + k) % n;
      cout << "c_addr and shortcut_addr: " << nodei->getAddress() << "," << shortcut_address << endl;
      RandAddrNode* nodej = node_vec[shortcut_address];
      Network* nei_net = getNeighbors(nodei);
      cout << "getEdge():\t" << (getEdge(nodei,nodej)==0)<<  endl;
      cout << "nodei!= nodej?\t" << (nodei!=nodej) << endl;
      /*if ((nodei->getAddress() != nodej->getAddress()) && !(getEdge(nodei, nodej)) && !(getEdge(nodej, nodei))){*/
      
      //if ((nodei != nodej) && (nei_net->getEdge(nodei, nodej)!=0)){
      if ((nodei!=nodej) && (getEdge(nodei, nodej)==0)){
          //add(Edge(nodei,nodej));
	  //c_edge = new SWEdge(nodei,nodej, "SC");
	  cout << getEdge(nodei,nodej) << endl;
          add(SWEdge(nodei,nodej, "SC"));
	  //add(*c_edge);
	  cout << "-----------------------------------------" << endl;
	  cout << "(" << nodei->getAddress() << "," << nodej->getAddress() << ")" << endl;
      }
    }
    cout << "How manay edges? " << getEdgeSize() << endl;
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
        //cout << "(" << noi->getAddress() << "," << noj->getAddress() <<  ")" << endl;
      }
    }// delete c_edge;
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
	   //cout << "[" << fnode->getAddress() << "," << snode->getAddress() << "," << c_edge->getAttributes() << "," << endl;
	   cout << "[" << fnode->getAddress() << "," << snode->getAddress() << "]" << endl;
	   delete c_edge;
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
