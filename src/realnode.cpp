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

#include "realnode.h"
#include "mrucache.h"
#include "realcontentnetwork.h"

using namespace std;
using namespace Starsky;

//#define DEBUG

RealNode::RealNode() {
  global_trust_score = 0.0;
  max_size = 500; //max cache size is 500, experimental number
  local_cache = MruCache(max_size);
}

RealNode::RealNode(const std::string& s, int max) : Node(s) {
  global_trust_score = 0.0;
  max_size = max; //max cache size is 500, experimental number
  local_cache = MruCache(max_size);
}

void RealNode::setScore(double new_score){
  global_trust_score = new_score;
}

void RealNode::implantPublicCache(Node* source_node, std::set<ContentNode*> contents ){
	source_nodes_set.insert(source_node);

	set<ContentNode*>::const_iterator c_it;
	for(c_it = contents.begin(); c_it != contents.end(); c_it++){
	  
	  content_to_source_nodes[*c_it].insert(source_node);

	  source_node_to_contents[source_node].insert(*c_it);

	  cached_content_set.insert(*c_it);

	}//end of for-loop

}

void RealNode::insertLocalCache(ContentNode* content, double current_time){
  try{
    local_cache.insert(content, "null", current_time);
    cached_content_set.insert(content);
  }
  catch(...){
    cout << "Error inserting content in local cache realnode.cpp insertLocalCache()" << endl;
    abort();
  }
}

void RealNode::print(){
#ifdef DEBUG
  cout << "Printing RealNode: " << Node::toString() << endl;

  std::map< ContentNode*, Network::NodePSet>::const_iterator it1;
  cout << "Printing Content to Source Nodes map" << endl;
  
  if(!content_to_source_nodes.empty()){
  for(it1 = content_to_source_nodes.begin(); it1 != content_to_source_nodes.end(); it1++){
    cout << (*it1).first->getID() << " : " << '\t';
    Network::NodePSet temp_set =  (*it1).second;
    Network::NodePSet::const_iterator s_it;
    for (s_it = temp_set.begin(); s_it != temp_set.end(); s_it++){
      cout << (*s_it)->toString() << " ";
    }
    cout << endl;
  }
  }
  else{
    cout << "Content to Source Nodes map is empty" << endl;
  }
  cout << endl;

  std::map<Node*, std::set<ContentNode*> >::const_iterator it2;
  cout << "Printing Source to Contents map" << endl;
  if(!source_node_to_contents.empty()){
  for(it2 = source_node_to_contents.begin(); it2 != source_node_to_contents.end(); it2++){
    cout << (*it2).first->toString() << " : " << '\t';
    std::set<ContentNode*> temp_set =  (*it2).second;
    std::set<ContentNode*>::const_iterator s_it;
    for (s_it = temp_set.begin(); s_it != temp_set.end(); s_it++){
      cout << (*s_it)->getID() << " ";
    }
    cout << endl;
  }
  }
  else{
    cout << "Source to Contents map is empty" << endl;
  }
  cout << endl;
  
  Network::NodePSet::const_iterator it3;
  cout << "Printing the set of source nodes" << endl;
  if(!source_nodes_set.empty()){
    for(it3 = source_nodes_set.begin(); it3 != source_nodes_set.end(); it3++){
      cout << (*it3)->toString() << ", ";
    }
    cout << endl;
  }
  else{
    cout << "Source Nodes set is empty" << endl;
  }
  cout << endl;

  std::set<ContentNode*>::const_iterator it4;
  cout << "Printing the set of contents" << endl;
  if(!cached_content_set.empty()){
    for(it4 = cached_content_set.begin(); it4 != cached_content_set.end(); it4++){
      cout << (*it4)->getID() << ", ";
    }
    cout << endl;
  }
  else{
    cout << "Cached content set is empty" << endl;
  }
  cout << endl;

  local_cache.print();
#endif
}

void RealNode::updateLocalCache(ContentNode* content, double current_time){
  try{
    local_cache.update(content, current_time);
  }
  catch(...){
    cout << "Error updating content in local cache realnode.cpp updateLocalCache()" << endl;
    abort();
  }
}

void RealNode::removePublicCache(Node* source_node, std::set<ContentNode*> contents ){

	set<ContentNode*>::const_iterator c_it;
	for(c_it = contents.begin(); c_it != contents.end(); c_it++){	  

	  source_node_to_contents[source_node].erase(*c_it);
	  if(source_node_to_contents[source_node].empty() ){
	    ///first, erase the entry on the source_node_to_contents map
	    source_node_to_contents.erase(source_node);
    	    ///second, erase the entry on the source_nodes_set
	    source_nodes_set.erase(source_node);
	  }

	  content_to_source_nodes[*c_it].erase(source_node);
	  if(content_to_source_nodes[*c_it].empty() ){
	    ///first, erase the entry on the content_to_source_nodes map
	    content_to_source_nodes.erase(*c_it);
    	    ///second, erase the entry on the source_nodes_set
	    cached_content_set.erase(*c_it);
	  }
	  
	}//end of for-loop
}

void RealNode::updateLTV(){
  std::map<Node*, int>::iterator m_it;
  int total = 0, temp;
  for(m_it = node_to_trust_temp.begin(); m_it != node_to_trust_temp.end(); m_it++){
    temp = m_it->second;
    if( node_to_trust.find( m_it->first) != node_to_trust.end()){ //node already exists
      node_to_trust[m_it->first] += temp; //check if the node already exist!
    }
    else{ //adding the node
      node_to_trust[m_it->first] = temp;
    }    
  }//finish updating the new node_to_trust vector

  //begin computation of the new local trust vector
  for(m_it = node_to_trust.begin(); m_it != node_to_trust.end(); m_it++){
    total += m_it->second;
  }

  local_trust_vector.clear();

  for(m_it = node_to_trust.begin(); m_it != node_to_trust.end(); m_it++){
    local_trust_vector[m_it->first] = (double)m_it->second/(double)total;
  }
  node_to_trust_temp.clear(); //clear the temporary record of trusted nodes in this sim cycle
}

void RealNode::updateTempNodeToTrust(Node* node){
  if(node_to_trust_temp.find(node) != node_to_trust_temp.end() ){
    node_to_trust_temp[node]++;
  }
  else{
    node_to_trust_temp[node] = 1;
  }
}
