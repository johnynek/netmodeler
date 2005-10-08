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

const Network& ContentNetwork::getNodesHoldingContent(ContentNode* c) const {
    map<ContentNode*,Network>::const_iterator c_it;
    c_it = _content_to_nodes.find(c);
    if( c_it != _content_to_nodes.end() ) {
      
      return c_it->second;
    }
    else {
      return Network::_empty_net;
    }
}

void ContentNetwork::insertContent(ContentNode* content, NodeIterator* ni) {

    _content_set.insert(content);
    while( ni->moveNext() ) {
      Node* this_node = ni->current();
      _content_map[this_node].insert(content);
      _content_to_nodes[content].add( this_node );
    }
}

Network* ContentNetwork::queryForContent(ContentNode* content, NodeIterator* ni) {
    //The first thing we do is forget where this message has been:
    Network* ret_val = new Network();
    while( ni->moveNext() ) {
      Node* this_node = ni->current();
      if( _content_map[this_node].find(content) != _content_map[this_node].end()) {
        ret_val->add( this_node );
      }
    }
    return ret_val;
}
