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

#ifndef starsky__realnode
#define starsky__realnode

#include "node.h"
#include "network.h"
#include "contentnode.h"
#include "cache.h"
#include "mrucache.h"

namespace Starsky {

/**
 * The RealNode's will be attached to the RealContentNetwork
 * @see Starsky::RealContentNetwork
 */
	
class RealNode : public Node {
  
 public:

  RealNode();
  /**
   * constructor
   * @param s the name of the realnode
   * @param max the size of the realnode's local cache
   */
  RealNode(const std::string& s, int max);

  ///The following are constant access functions
  /**
   * @return max_size, which is the size of realnode's local cache
   */
  int getMaxSize() const { return max_size;}
  /**
   * @return the global trust score of the realnode
   */
  double getScore() const { return global_trust_score;}

  /**
   * @return the map that maps a content to all the source nodes with that content
   */
  std::map< ContentNode*, Network::NodePSet > getCTSMap() const { return content_to_source_nodes;}
  /**
   * @return the map that maps a source node and all its content in this realnode
   */
  std::map<Node*, std::set<ContentNode*> > getSTCMap() const { return source_node_to_contents;}
  /**
   * @return a set of nodes that have cached in this realnode
   */
  Network::NodePSet getSourceNodes() const { return source_nodes_set;} 
  /**
   * @return a set of all cached contents (from all source nodes) at this realnode
   */
  std::set<ContentNode*> getCCSet() const { return cached_content_set;}
  /**
   * @return the local cache (Most Recently Used) of this realnode
   */
  MruCache getLocalCache() const{ return local_cache;}
  /**
   * @return the Local Trust Vector 
   */
  std::map<Node*, double> getLTV() const {return local_trust_vector;}

  ///The following are "modify" functions

  void clearNewInsertKeys() {local_cache.clearNewInsertKeySet();}

  void clearRemovedKeys() {local_cache.clearRemovedKeySet();}
  /**
   * @param content, the content to be inserted to the local cache of the realnode
   * @param current_time the time stamp that the insert of content takes place
   */
  void insertLocalCache(ContentNode* content, double current_time);
  /**
   * @param source_node, the node that is trying to implant contents
   * @param contents the set of contents to be implanted
   * Note that the public cache is implemented through two std::map's and two std::set's
   */
  void implantPublicCache(Node* source_node, std::set<ContentNode*> contents);

  void print();

  /**
   * @param source_node, the node that is trying to remove contents
   * @param contents the set of contents to be removed
   * Note that the public cache is implemented through two std::map's and two std::set's
   */
  void removePublicCache(Node* source_node, std::set<ContentNode*> contents);

  /**
   * @param new_score the new global trust score to be assigned to the realnode
   */
  void setScore(double new_score);
  /**
   * @param content, the content to be updated on the local cache of the realnode
   * @param current_time the time stamp that the update of content takes place
   */
  void updateLocalCache(ContentNode* content, double current_time);
  /**
   * this function updates the local trust vector after a run of simulation using data
   * from the temporary local trust vector
   */
  void updateLTV();

  /**
   * Add one more unit of trust to node by modifying the node_to_trust_temp;
   */
  void updateTempNodeToTrust(Node* node);

 protected: 

  /**
   * maps each content to all the nodes that have cached there
   */
  std::map< ContentNode*, Network::NodePSet> content_to_source_nodes;
  /**
   * maps each source node to its cache at this node
   */
  std::map<Node*, std::set<ContentNode*> > source_node_to_contents;
  /**
   * a list of source nodes that have cached at this node
   */
  Network::NodePSet source_nodes_set;
  /**
   * a full list of all contents that are cached at the node
   */
  std::set<ContentNode*> cached_content_set;
  /**
   * a list of nodes trusted and degree of trust for each node tabulated for one run of simulation
   * IMPORTANT: clear the this map at the end of each simulation
   */
  std::map<Node*, int> node_to_trust_temp;
  /**
   * a list of nodes trusted and degree of trust for each node tabulated from beginning of simulation
   */
  std::map<Node*, int> node_to_trust;
  /**
   * a list of nodes trusted and fraction of trust for each node tabulated from beginning
   */
  std::map<Node*, double> local_trust_vector;
  /**
   * The global trust value for this node after the EigenTrust computation
   */
  double global_trust_score;
  /**
   * The maximum size of the cache size for each node
   */
  int max_size;

  MruCache local_cache;

};
	
}

#endif

