#include "contentnetwork.h"

using namespace std;
using namespace Starsky;



ContentNetwork::ContentNetwork(Network& aNet) : _my_net(aNet) {

}

void ContentNetwork::deleteContent() {
    set<ContentNode*>::iterator c_it;
    for(c_it = _content_set.begin(); c_it != _content_set.end(); c_it++) {
        delete *c_it;
    }
    _content_set.clear();
    _content_map.clear();
    _content_to_nodes.clear();
}

const set<ContentNode*>& ContentNetwork::getContent() const {
    return _content_set;
}

const Network::NodePSet& ContentNetwork::getNodesHoldingContent(ContentNode* c) const {
    map<ContentNode*,Network::NodePSet>::const_iterator c_it;
    c_it = _content_to_nodes.find(c);
    if( c_it != _content_to_nodes.end() ) {
      return c_it->second;
    }
    else {
      return Network::_empty_nodeset;
    }
}

void ContentNetwork::insertContent(Node* node, ContentNode* content, Message& amessage) {

    _content_set.insert(content);
    //The first thing we do is forget where this message has been:
    amessage.forgetVisitedNodes();
    amessage.visit(node, _my_net);
    const Network::NodePSet& content_getters = amessage.getVisitedNodes();
    Network::NodePSet::const_iterator n_it;
    for(n_it = content_getters.begin(); n_it != content_getters.end(); n_it++) {
        _content_map[*n_it].insert(content);
	_content_to_nodes[content].insert( *n_it );
    }
    
}

Network::NodePSet ContentNetwork::queryForContent(Node* node,
		                                  ContentNode* content,
						  Message& amessage) {
    //The first thing we do is forget where this message has been:
    amessage.forgetVisitedNodes();
    amessage.visit(node, _my_net);
    const Network::NodePSet& content_searchers = amessage.getVisitedNodes();
    Network::NodePSet::const_iterator n_it;
    Network::NodePSet ret_val;
    for(n_it = content_searchers.begin(); n_it != content_searchers.end(); n_it++) {
        if( _content_map[*n_it].find(content) != _content_map[*n_it].end()) {
            ret_val.insert( *n_it );
	}
    }
    return ret_val;
}
