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

#include "gnutellafilespacenetwork.h"

using namespace Starsky;
using namespace std;

const int GnutellaFileSpaceNetwork::content_type = 0;
const int GnutellaFileSpaceNetwork::node_type = 1;
const int GnutellaFileSpaceNetwork::query_type = 2;

GnutellaFileSpaceNetwork::GnutellaFileSpaceNetwork(istream& file, int max) {

  SuperString line;
  vector<SuperString> tmp;
  int nodes = 0;
  while(getline(file, line, '\n') && (nodes < max)) {
      tmp = line.split(": ");
      if( tmp[0] == "Servent" ) {
        //This is a queryhit:
	nodes += readQueryHit(file,tmp[1]);
      }
      else if( tmp[0] == "QueryMID") {
	//This is a query:
        //nodes += readQuery(file,tmp[1]);
	/*
	 * We are skipping recording queries since the query
	 * hit will make a node for the query if there is not
	 * already.  This makes sure that we only keep a node
	 * for queries that have at least one response (most
	 * of which do not).
	 */
         while(getline(file,line,'\n') && (line != "") ) {
         }
      }
  }
}

map<double,int> GnutellaFileSpaceNetwork::getContentWeights() const {
  map<double,int> weights;
  const NodePSet& content = getNodes(content_type);
  NodePSet::const_iterator it;
  ConnectedNodePSet::const_iterator nit;
  map<int,NodePSet>::const_iterator mit;
  int node_neighbors, query_neighbors;
  
  for( it = content.begin(); it != content.end(); it++) {
    node_neighbors = 0;
    query_neighbors = 0;
    auto_ptr<NodeIterator> ni( getNeighborIterator(*it) );
    while(ni->moveNext()) {
      Node* this_node = ni->current();
      mit = _type_to_node_map.find(node_type);
      if(mit != _type_to_node_map.end()) {
	if( mit->second.find(this_node) != mit->second.end()) {
           ++node_neighbors;
	}
      }
      mit = _type_to_node_map.find(query_type);
      if(mit != _type_to_node_map.end()) {
	if( mit->second.find(this_node) != mit->second.end()) {
           ++query_neighbors;
	}
      } 
    }
    weights[ ((double)node_neighbors)/((double)query_neighbors) ]++;
  }
  return weights;
}

int GnutellaFileSpaceNetwork::readQuery(istream& file, const string& mid) {

  int new_count = 0;
  //Read one query:
  map<string,GnutellaQuery*>::iterator qit = _query_by_mid.find(mid);
  GnutellaQuery* q;
  if( qit != _query_by_mid.end()) {
      q = qit->second; 
  }
  else {
      q = new GnutellaQuery(mid);
      ++new_count;
      _query_by_mid[mid] = q;
  }
  NPartiteNetwork::add(query_type,q);
  //Assume we only see each query once.
  SuperString line;
  while(getline(file,line,'\n') && (line != "") ) {
    //Ignore the query information for now
  }
  return new_count;
}

int GnutellaFileSpaceNetwork::readQueryHit(istream& file, const string& sid) {

  int new_count = 0;
	
  GnutellaNode* node = 0;
  GnutellaContent* content = 0;
  GnutellaQuery* q = 0;
 
  //Get the node:
  map<string, GnutellaNode*>::const_iterator n_it = _node_by_name.find(sid);
  if( n_it == _node_by_name.end() ) {
      //This is a new node:
      node = new GnutellaNode(sid);
      ++new_count;
      NPartiteNetwork::add(node_type,node);
      _node_by_name[sid] = node;
  }
  else {
    node = n_it->second;
  }

  SuperString line;
  vector<SuperString> tmp;
  map<string, GnutellaContent*>::const_iterator c_it;
  
  string last_file, last_sha1, last_muid;
  int last_size=0;
  
  while(getline(file, line, '\n') && (line != "")) {
    /*
     * hits start with:
     * Servent:
     * and follow by any number of {File:.*\nSize:.*\n[Extension:.*]?}
     */
      tmp = line.split(": ");
      if( tmp[0] == "File" ) {
          last_file = tmp[1];
      }
      else if( tmp[0] == "Size" ) {
          last_size = atoi( tmp[1].c_str() );
      }
      else if( tmp[0] == "Muid" ) {
          last_muid = tmp[1];
      }
      else if( tmp[0] == "Extension" ) {
	  string::size_type pos = tmp[1].find("urn:sha1");
	  if( pos != string::npos ) {
	    last_sha1 = tmp[1].substr(pos+8,32);
	    c_it = _content_by_hash.find( last_sha1 );
	    if( c_it != _content_by_hash.end() ) {
                content = c_it->second;
	    }
	    else {
                content = new GnutellaContent(last_sha1,last_file,last_size);
		++new_count;
		_content_by_hash[last_sha1] = content;
		NPartiteNetwork::add(content_type, content);
	    }

            if( content->getSize() != last_size) {
/*
                    cerr << "possible spam: servent: " << node->toString()
			 << " file: "
			 << last_file << " size: " << last_size << " prev file: "
			 << *(content->getFileNameSet().begin())
			 << " size: " << content->getSize() << endl;
*/
	    }
	    else {
                if( content->getFileNameSet().count(last_file) == 0 ) {
                      content->addFileName( last_file );
                }
		//Add the file:
		Network::add(Edge(node,content));
		//Look up the query this is a link for:
	        map<string,GnutellaQuery*>::iterator qit =
			                    _query_by_mid.find(last_muid);
		if( qit != _query_by_mid.end()) {
		  q = qit->second; 
		}
		else {
                  q = new GnutellaQuery(last_muid);
		  NPartiteNetwork::add(query_type, q);
		  ++new_count;
		  _query_by_mid[last_muid] = q;
		}
		//This content is connected to the query q.
		Network::add(Edge(q,content));
	    }
	  }
	  
      }
  }
  return new_count; 
}
