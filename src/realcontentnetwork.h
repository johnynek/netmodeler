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

#ifndef starsky__realcontentnetwork
#define starsky__realcontentnetwork

#include "network.h"
#include "node.h"
#include "contentnode.h"
#include "contentnetwork.h"
#include "message.h"
#include "realnode.h"
#include <map>

namespace Starsky {

 /**
  * Created from an existing network, this class keeps track
  * of the "real" content associated with nodes.  This allows modeling of
  * queries and caching on P2P systems.  Content is modeled as a node.
  * This class maintains a set of "RealNode"'s that contains all the detail information
  * @see Starsky::ContentNode
  * @see Starsky::RealNode
  * IMPORTANT: we need to change the inheritance soon, RealContentNetwork should inherit from
  * ContentNetwork
  */
	
class RealContentNetwork : public Network {

    public:
	/**
	 * @param aNet the network topology to use for the nodes
	 */
        RealContentNetwork(Network& aNet);
	
	/**
	 * @param node the node that we are interested in
	 * @return the RealNode that corresponds to node for modification
	 */
	RealNode* obtainRealNode(Node* node) {return node_to_real_node[node];}
       	/**
	 * @param node the node that inserts the content.
	 * @param content_set the set of contents to be inserted
	 * @param target_nodes these nodes get the content from 'node'
	 */
	void insertContent(Node* node, std::set<ContentNode*> content_set, NodeIterator* target_nodes);
       	/**
	 * @param node the node that inserts the content.
	 * @param content_set the set of contents to be inserted
	 * @param amessage the message which starts at node, and all visited nodes get content
	 */
	void insertContent(Node* node, std::set<ContentNode*> content_set, Message& amessage);
	/**
	 * @param node the node to start the query at
	 * @param content the content we are searching for
	 * @param amessage the message to carry the query
	 * @param time_now the current time that the query is performed
	 * @return set of nodes which are reached by the message and have the content
	 */
	Network::NodePSet queryForContent(Node* node, ContentNode* content, Message& amessage, double time_now);

	/**
	 * @param cached_nodes the set of nodes that have content on their cache
	 * @param content the content that we are interested here
	 * @return set of "source nodes" that have cached the content on the set of cached nodes)
	 */
	Network::NodePSet getSourceNodes(Network::NodePSet cached_nodes, ContentNode* content);

	std::map<Node*, RealNode*> getNodeMap(){return node_to_real_node;}

	std::set<RealNode*> getRealNodes();

	/** 
	 * @param source_nodes the set of nodes that we are interested in
	 * @param min_trust the score given to a node with a trust score less than min_trust
	 * @return the total trust score of the set of source_nodes (i.e. sum of individual 
	 * global trust values of the set of nodes)
	 */
	double getTotalTrust(Network::NodePSet source_nodes, double min_trust);      

	/**
	 * @param accessed_nodes the set of nodes that have the content on their public cache 
	 * @param content the content that was accessed,
	 * @param new_time the time stamp that the content is accessed
	 * note that the source_nodes will be obtained via the getSourceNodes fcn and
	 * the source_nodes will be contacted
	 */
	void notifyAccess(Network::NodePSet accessed_nodes, ContentNode* content, double new_time); 

       	/**
	 * @param node the node that initiates the removal of the content set
	 * @param content_set the set of contents to be removed
	 * @param target_nodes these nodes will remove the stuff corresponding to source 'node'
	 * and the content_set to be removed
	 */
	void removeContent(Node* node, std::set<ContentNode*> content_set, Network::NodePSet target_nodes);

    protected:
	/**
	 * Reference to the network with the nodes in it
	 */
	Network& _my_net;
	/**
	 * Provides a mapping from node to RealNode
	 */
	std::map<Node*, RealNode*> node_to_real_node;	
	
};
	
}

#endif

