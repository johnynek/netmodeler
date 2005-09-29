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

#include "realcontentnetwork.h"
#include "network.h"
#include "realnode.h"
#include "mrucache.h"
#include <stdexcept>

//#define DEBUG

using namespace std;
using namespace Starsky;

RealContentNetwork::RealContentNetwork(Network& aNet) : _my_net(aNet) {
  Network::NodePSet::const_iterator n_it;
  int cache_size = 300; //every node has max cache size of 'cache_size'
  NodeIterator ni = _my_net.getNodeIterator();
  while( ni.moveNext() ) {
    Node* n = ni.current();
    RealNode* rnode = new RealNode( n->toString(), cache_size ); 
    node_to_real_node[n] = rnode;
  }
}

std::set<RealNode*> RealContentNetwork::getRealNodes(){
  std::set<RealNode*> ret_val;
  std::map<Node*, RealNode*>::const_iterator m_it;
  for(m_it = node_to_real_node.begin(); m_it != node_to_real_node.end(); m_it++){
    ret_val.insert( (*m_it).second);
  }
  return ret_val;
}

Network::NodePSet RealContentNetwork::getSourceNodes(Network::NodePSet cached_nodes, ContentNode* content){
  
  Network::NodePSet ret_val;
  Network::NodePSet::const_iterator cn_it;
  Network::NodePSet::const_iterator n_it;
  for(cn_it = cached_nodes.begin(); cn_it != cached_nodes.end(); cn_it++){
    RealNode* rnode = node_to_real_node[*cn_it];
    Network::NodePSet local_source_set = rnode->getCTSMap()[content];
    for(n_it = local_source_set.begin(); n_it != local_source_set.end(); n_it++){
      ret_val.insert(*n_it);
    }
  }
  return ret_val;
}

double RealContentNetwork::getTotalTrust(Network::NodePSet source_nodes, double min_trust){
  double tot_trust = 0.0;
  Network::NodePSet::const_iterator n_it;
  for(n_it = source_nodes.begin(); n_it != source_nodes.end(); n_it++){
    double score = node_to_real_node[*n_it]->getScore();
    if(score < min_trust){
      tot_trust += min_trust;   
    }
    else{
      tot_trust += score;
    }
  }
  return tot_trust;
}

void RealContentNetwork::insertContent(Node* node, std::set<ContentNode*> content_set, NodePSet target_nodes){

    Network::NodePSet::const_iterator n_it;
    for(n_it = target_nodes.begin(); n_it != target_nodes.end(); n_it++) {

	node_to_real_node[*n_it]->implantPublicCache(node, content_set);

    }//end of outer for-loop
    node_to_real_node[node]->clearNewInsertKeys();
}

void RealContentNetwork::insertContent(Node* node, set<ContentNode*> content_set, Message& amessage) {

    //The first thing we do is forget where this message has been:
    amessage.forgetVisitedNodes();
    amessage.visit(node, _my_net);
    Network::NodePSet nodes_visited = amessage.getVisitedNodes();
    try{
      RealContentNetwork::insertContent(node, content_set, nodes_visited);
    }
    catch(...){
      cout << "Error in insertContent realcontentnetwork.cpp" << endl;
      abort();
    }
}

void RealContentNetwork::notifyAccess(Network::NodePSet accessed_nodes, ContentNode* content, double new_time){
  ///Note that we need to notify to only the source nodes that have the content in its cache
  Network::NodePSet nodes_to_notify = RealContentNetwork::getSourceNodes(accessed_nodes, content);
  Network::NodePSet::const_iterator n_it;
  for(n_it = nodes_to_notify.begin(); n_it != nodes_to_notify.end(); n_it++){    
#ifdef DEBUG
    cout << "nodes_to_notify has: " << (*n_it)->toString() << " and the content is: " << content->getID() << endl;
    cout << "And the time is: " << new_time << "  PRINTING the cache" << endl;
    node_to_real_node[*n_it]->getLocalCache().printtimekey();
    node_to_real_node[*n_it]->getLocalCache().printkeytime();
#endif
    node_to_real_node[*n_it]->updateLocalCache(content, new_time); 
#ifdef DEBUG
    cout << "PRINTING cache again: " << endl;
    node_to_real_node[*n_it]->getLocalCache().printtimekey();
    node_to_real_node[*n_it]->getLocalCache().printkeytime();
#endif  
  }
}

Network::NodePSet RealContentNetwork::queryForContent(Node* node, ContentNode* content, Message& amessage, double time_now){

    //The first thing we do is forget where this message has been:
    amessage.forgetVisitedNodes();
    amessage.visit(node, _my_net);
    const Network::NodePSet& content_searchers = amessage.getVisitedNodes();
    Network::NodePSet::const_iterator n_it;
    Network::NodePSet ret_val;
    std::set<ContentNode*>::iterator c_it;
    for(n_it = content_searchers.begin(); n_it != content_searchers.end(); n_it++) {
      RealNode* rnode = node_to_real_node[*n_it];
#ifdef DEBUG
	cout << "One node found is: " << (*n_it)->toString() << endl;
	//rnode->print();
#endif
      c_it = rnode->getCCSet().find(content);
      if(c_it != rnode->getCCSet().end() ){//BIG PROBLEM HERE!!
#ifdef DEBUG
	cout << "The content is: " << content->getID() << " and content cached is: " << (*c_it)->getID() << endl;
	//rnode->print();
#endif
	ret_val.insert(*n_it);
      }
    }
    //notify this set of nodes in ret_val that their content was being accessed so that they can change the 
    //time stamps on their cache accordingly
#ifdef DEBUG
    cout << "got here: rcn 1." << endl;
#endif
    if(!ret_val.empty() ){
#ifdef DEBUG
      cout << "Size of ret_val is (rcn): " << ret_val.size() << endl;
#endif
      notifyAccess(ret_val, content, time_now); //notice that ret_val is a set of "cached nodes"
    }
#ifdef DEBUG
    cout << "got here: rcn 2." << endl;
#endif
    return ret_val;
}

void RealContentNetwork::removeContent(Node* node, std::set<ContentNode*> content_set, NodePSet target_nodes){

    Network::NodePSet::const_iterator n_it;

    for(n_it = target_nodes.begin(); n_it != target_nodes.end(); n_it++) {

	node_to_real_node[*n_it]->removePublicCache(node, content_set);

    }//end of outer for-loop
    node_to_real_node[node]->clearRemovedKeys();
}
