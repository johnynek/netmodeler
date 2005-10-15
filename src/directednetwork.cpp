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

#include "directednetwork.h"
#include "directededge.h"

using namespace Starsky;
using namespace std;

DirectedNetwork::DirectedNetwork() : Network() { }

DirectedNetwork::DirectedNetwork(std::istream& in){
		readFrom(in);
	}


bool DirectedNetwork::add(const Edge& edge) {
  try {
    
    const DirectedEdge& d_e = dynamic_cast<const DirectedEdge&>(edge);
    
    Edge* n_edge = getEdgePtr(d_e);

    if( n_edge == 0) {
      //This is a new edge:
	    DirectedEdge* d_edge = new DirectedEdge(d_e);
	    n_edge = d_edge;
    }
    return add(n_edge);
  }
  catch(bad_cast x) {
    return false;
  }
  return false;
}

bool DirectedNetwork::add(Edge* aEdge) {
  DirectedEdge* d_edge = dynamic_cast<DirectedEdge*>(aEdge);
  if( d_edge == 0) {
    //This is not a directed edge:
    return false;  
  }
  
  if( has(*d_edge) == true) {
    //We don't add edges twice
    return false;
  }
  //Else, let Network::add do the job
  Network::add(d_edge);
}

///\todo: fix the below!
void DirectedNetwork::moveIntoUndirectedNetwork(Network& net) {

  auto_ptr<NodeIterator> ni( getNodeIterator() );
  while( ni->moveNext() ) {
    net.add( ni->current() );
  }
  auto_ptr<EdgeIterator> ei( getEdgeIterator() );
  while( ei->moveNext() ) {
    Edge* this_edge = ei->current();
    net.add( Edge( this_edge->first, this_edge->second) );
  }
  //Empty out this network:
  clear();
}

double DirectedNetwork::getAssortativity() const {

  //See eq. 25 in cond-mat/0209450
  int j,k;
  double sum_j, sum_k, sum_jk, sum_j2, sum_k2;

  //These are double because for large networks, they will be more than 2^32
  sum_j = 0.0;
  sum_k = 0.0;
  sum_jk = 0.0;
  sum_j2 = 0.0;
  sum_k2 = 0.0;
  EdgeSet::const_iterator i;
  DirectedEdge* d_edge;
  auto_ptr<EdgeIterator> ei( getEdgeIterator() );
  while(ei->moveNext()) {
    //We need "remaining degree" for this calculation
    d_edge = dynamic_cast<DirectedEdge*>( ei->current() );
    //Make sure that j has the start node:
    if( d_edge->pointsFirstToSecond() ) {
      j = getOutDegree( d_edge->first ) - 1;
      k = getInDegree( d_edge->second ) - 1;
    }
    else {
      j = getOutDegree( d_edge->second ) - 1;
	    k = getInDegree( d_edge->first ) - 1;
    }
    sum_j += (double)j;
    sum_k += (double)k;
    sum_jk += (double)(j * k);
    sum_j2 += (double)(j * j);
    sum_k2 += (double)(k * k);
  }
  double r, m_inv = 1.0 / (double)(getEdgeSize());

  /**
   *
   * r = frac{(sum_i j_i k_i) - M^{-1}(sum_i j_i)(sum_i k_i)}
   *          {sqrt{[(sum_i j_i^2) - M^{-1}(sum_i j_i)^2]
   *                    [(sum_i k_i^2) - M^{-1}(sum_i k_i)^2]}
   * 
   */

  r = ((double)sum_jk - m_inv * (double)(sum_j * sum_k))
	  / sqrt( ((double)sum_j2 - m_inv * (double)(sum_j * sum_j)) *
	          ((double)sum_k2 - m_inv * (double)(sum_k * sum_k)) );
  return r;
	
}

#ifndef HIDE_STL
const Network::EdgeSet& DirectedNetwork::getEdges() const {
  
  return edge_set;
}
#endif

Edge* DirectedNetwork::getEdgePtr(const Edge& edge) const {

  try {
  	const DirectedEdge& d_e = dynamic_cast<const DirectedEdge&>(edge);
		Node* start;
		Node* end;
		if( d_e.pointsFirstToSecond() ) {
      start = d_e.first;
	  	end = d_e.second;
		}
		else {
    	start = d_e.second;
	  	end = d_e.first;
		}
	
		EdgeSet::const_iterator eit1, eit2;
		auto_ptr<EdgeIterator> ei1( getEdgeIterator( start ) );
		while( ei1->moveNext() ) {
		  Edge* edge1 = ei1->current();
	          DirectedEdge* d_edge1 = dynamic_cast<DirectedEdge*>(edge1);
	          auto_ptr<EdgeIterator> ei2( getEdgeIterator( end ) );
		  while( ei2->moveNext() ) {
	            Edge* edge2 = ei2->current();
		    DirectedEdge* d_edge2 = dynamic_cast<DirectedEdge*>(edge2);
	            if( d_edge1 == d_edge2 && d_edge1->getStartNode() == start ) {
		      return d_edge1;
		    }
	          }
		}
		return 0;  
	}
  catch(bad_cast x) {
  	//This is not a directed edge.
		return 0;  
	}
}

map<int, int> DirectedNetwork::getInDegreeDist(NodeIterator* nodes) const {

  map<int, int> deg_dist;
  int deg = 0;

  while( nodes->moveNext() ) {
    Node* this_node = nodes->current();
    deg = getInDegree( this_node );
    deg_dist[deg] = deg_dist[deg] + 1;
  }
  return deg_dist;
}

map<int, int> DirectedNetwork::getInDegreeDist() const {
  auto_ptr<NodeIterator> ni(getNodeIterator());
  return getInDegreeDist( ni.get() );
}

int DirectedNetwork::getInDegree(Node* node) const {
  if( has( node ) ) {
  int in_deg = 0;
    auto_ptr<EdgeIterator> ei( getEdgeIterator(node) );
    while( ei->moveNext() ) {
      Edge* this_edge = ei->current();
      DirectedEdge* d_e = dynamic_cast<DirectedEdge*>(this_edge);
      if( d_e->getEndNode() == node ) {
        //This is an out-degree;
        in_deg++;
      }
    }
    return in_deg;
  }
  else {
    //Node not in the netork:
    return -1;
  }
}

map<int, int> DirectedNetwork::getOutDegreeDist(NodeIterator* nodes) const {
  map<int, int> deg_dist;
  int deg = 0;

  while( nodes->moveNext() ) {
    Node* this_node = nodes->current();
    deg = getOutDegree( this_node );
    deg_dist[ deg ] = deg_dist[deg] + 1;
  }
  return deg_dist;
}

map<int, int> DirectedNetwork::getOutDegreeDist() const {
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  return getOutDegreeDist( ni.get() );
}

int DirectedNetwork::getOutDegree(Node* node) const {
  if( has( node ) ) {
  int out_deg = 0;
    auto_ptr<EdgeIterator> ei( getEdgeIterator(node) );
    while( ei->moveNext() ) {
      Edge* this_edge = ei->current();
      DirectedEdge* d_e = dynamic_cast<DirectedEdge*>(this_edge);
      if( d_e->getStartNode() == node ) {
        //This is an out-degree;
        out_deg++;
      }
    }
    return out_deg;
  }
  else {
    //Node not in the netork:
    return -1;
  }
}

double DirectedNetwork::getInDegreeMoment(int m) const
{
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  return getInDegreeMoment(m, ni.get() );
}

double DirectedNetwork::getInDegreeMoment(int m, NodeIterator* nodes) const
{
  double ave = 0.0;
  int tot = 0;
  if( m == 1) {
    while( nodes->moveNext() ) {
      Node* this_node = nodes->current();
      ave += (double)getInDegree( this_node );
      tot++;
    }
  }
  else if( m == 2) {
    while( nodes->moveNext() ) {
      Node* this_node = nodes->current();
      ave += (double)(getInDegree( this_node ) * getInDegree( this_node ));
      tot++;
    }	
  }
  else {
    while( nodes->moveNext() ) {
      Node* this_node = nodes->current();
      ave += pow( (double)getInDegree( this_node ), m);
      tot++;
    }
  }
  return ave / (double)( tot );
}

double DirectedNetwork::getOutDegreeMoment(int m) const
{
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  return getOutDegreeMoment(m, ni.get() );
}

double DirectedNetwork::getOutDegreeMoment(int m, NodeIterator* nodes) const
{
  double ave = 0.0;
  int tot = 0;
  if( m == 1) {
    while( nodes->moveNext() ) {
      Node* this_node = nodes->current();
      tot++;
      ave += (double)getOutDegree( this_node );
    }
  }
  else if( m == 2) {
    while( nodes->moveNext() ) {
      Node* this_node = nodes->current();
      tot++;
      ave += (double)(getOutDegree( this_node ) * getOutDegree( this_node ));
    }	
  }
  else {
    while( nodes->moveNext() ) {
      Node* this_node = nodes->current();
      tot++;
      ave += pow( (double)getOutDegree( this_node ), m);
    }
  }
  return ave / (double)( tot );
}

void DirectedNetwork::printTo(ostream& out) const {

  /*
   * This is for use with graphviz, an AT&T graph visualization package
   * I had to cast the add some text ("n") so as not to confuse graphviz.
   */
	
  NodePSet::const_iterator i;
  string label;
  int node_count = 0;
  ///\todo fix this:
#if 0 
  out << "digraph g {" << endl;
  for(i = node_set.begin(); i != node_set.end(); i++) {
	  label = (*i)->toString();
	  if ( label != "" ) {
	    out << 'n' << *i << " [label=\"" << label 
        << "\",width=0.25,height=0.25,fontsize=9];" << endl;
    }
    else {
      out << 'n' << *i << " [label=\"" << node_count
        << "\",width=0.25,height=0.25,fontsize=9];" << endl;
	   }
    k = connection_map.find( *i );
	  if( k != connection_map.end() ) {
	    for(j = k->second.begin(); j != k->second.end(); j++) {
        out << 'n' << *i << " -> " << 'n' << *j << ";" << endl;
      }
    }
    node_count++;
  }
#endif
  out << "}" << endl;    
}

void DirectedNetwork::reverseEdges() {

  DirectedEdge* d_e;
  auto_ptr<EdgeIterator> ei( getEdgeIterator() );
  bool keep_going = ei->moveNext();
  while( keep_going ) {
      Edge* e = ei->current();
      keep_going = ei->moveNext();
      d_e = dynamic_cast<DirectedEdge*>( e );
      DirectedEdge* d_e2 = d_e->reverse();
      if( has( *d_e2) ) {
        /*
	 * The network already has the reversed edge,
	 * in this case do nothing, both of these
	 * would be reversed "into" each other.
	 *
	 * Delete this memory and forget about it
	 */
        delete d_e2;
      }
      else {
        /*
	 * The reversed edge is not in the network, so remove
	 * the current one and add the new one:
	 */
        remove(d_e); 
        add(d_e2);
      }
  }
}

#ifndef HIDE_STL
const DirectedNetwork::ConnectedNodePSet& DirectedNetwork::getOutNeighbors(Node* node) const {
  map< Node*, ConnectedNodePSet >::const_iterator i = connection_map.find(node);
  if( i != connection_map.end() ) {
    return i->second;
  }
  else {
    //returns an empty set
    return _empty_cnodeset;
  }
}

const DirectedNetwork::ConnectedNodePSet& DirectedNetwork::getInNeighbors(Node* node) const {
  map< Node*, ConnectedNodePSet >::const_iterator i = in_connection_map.find(node);
  if( i != in_connection_map.end() ) {
    return i->second;
  }
  else {
    //returns an empty set
    return _empty_cnodeset;
  }
}

int DirectedNetwork::getInComponentSize(Node* node) const {

	set<Node*> temp_in_component;
	temp_in_component.insert(node);
	
	set<Node*> done_in_component;
	
	DirectedNetwork::ConnectedNodePSet::iterator in_it;
	
	while ( temp_in_component.size() > 0 )
	{ 
	  in_it = temp_in_component.begin();
		
		while ( in_it != temp_in_component.end() )
		{
			DirectedNetwork::ConnectedNodePSet the_in_neighbors= getInNeighbors( *in_it );
			DirectedNetwork::ConnectedNodePSet::iterator in_t_it= the_in_neighbors.begin();
			DirectedNetwork::ConnectedNodePSet::iterator in_d_it;
			while(in_t_it != the_in_neighbors.end())
			{
				in_d_it = done_in_component.find(*in_t_it);
				if (in_d_it == done_in_component.end() )
					temp_in_component.insert( *in_t_it);
				
				in_t_it++;
			}
		  done_in_component.insert( *in_it );	
			temp_in_component.erase( *in_it );
			in_it++;
		}
	}

	return done_in_component.size();
}

set<DirectedNetwork> DirectedNetwork::getUndirectedComponents() const {

    set<DirectedNetwork> output;
    
    DirectedNetwork* tmp_net = 0;
    
    NodePSet to_check, checked;
    NodePSet::const_iterator i;
    NodePSet::iterator check_it;
    
    ConnectedNodePSet::const_iterator n_it;
    
    EdgeSet tmp_edges;
    EdgeSet::const_iterator e_it;
    
    //For all nodes
    for(i = node_set.begin(); i != node_set.end(); i++) {
	//Check to make sure we have not already identified this component:
	if( checked.find( *i ) == checked.end() ) {
            to_check.insert( *i );
            check_it = to_check.begin();
	    //Here we make a new component:
	    tmp_net = new DirectedNetwork();
            while( check_it != to_check.end() ) {
		tmp_net->add( *check_it );
                tmp_edges = Network::getEdges( *check_it );
	        for(e_it = tmp_edges.begin(); e_it != tmp_edges.end(); e_it++) {
                    tmp_net->add( *e_it );
	        }
	        for(n_it = getOutNeighbors( *check_it ).begin();
		    n_it != getOutNeighbors( *check_it ).end();
		    n_it++) {
		    if( checked.find( *n_it ) == checked.end() ) {
                      to_check.insert( *n_it );
		    }
		    //Else we have already checked this one.
	        }
	        for(n_it = getInNeighbors( *check_it ).begin();
		    n_it != getInNeighbors( *check_it ).end();
		    n_it++) {
		    if( checked.find( *n_it ) == checked.end() ) {
                      to_check.insert( *n_it );
		    }
		    //Else we have already checked this one.
	        }
	        checked.insert( *check_it );
                to_check.erase( check_it );
	        check_it = to_check.begin();
            }
	    checked.insert( *i );
	    //We have reached the entire component, go to the next.
	    output.insert( *tmp_net );
	    delete tmp_net;
	    tmp_net = 0;
        }
	else {
	}
    }
    return output;
}

#endif

void DirectedNetwork::setOutNodes(NodePSet& nodes) {
    NodePSet::const_iterator i;
     for(i = nodes.begin(); i != nodes.end(); i++) {
       	 if(DirectedNetwork::getOutDegree(*i) >= 1)
          out_node_set.insert(*i);	 
      }     
}

void DirectedNetwork::setOutNodes() {
  NodePSet node_set;
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  while( ni->moveNext() ) {
    node_set.insert( ni->current() );
  }
  setOutNodes(node_set);
}


void DirectedNetwork::readFrom(std::istream& in){
  SuperString line;
  map<string, Node*> name_map;
  while( getline(in, line, '\n') ) {
    if( line[0] == '#' ) { //This is a comment
      continue;
    }
    vector<SuperString> result = line.split(" : ");
    Node* first = 0;
    Node* second = 0;
    if( name_map.find(result[0]) == name_map.end() ) {
      //This is a new node:
      if( result[0] != "" ) {
        first = new Node(result[0]);
        name_map[ result[0] ] = first;
	Network::add( first );
      }
    }
    else {
      first = name_map[ result[0] ];
    }
    if( result.size() == 1 ) { //There was no neighbors
      continue;
    }
    result = result[1].split(" ");
    vector<SuperString>::iterator sit;
    for(sit = result.begin();
	sit != result.end();
	sit++) {
      second = 0;
      if( name_map.find( *sit ) == name_map.end() ) {
        //This is a new node:
	if( *sit != "" ) {
          second = new Node( *sit );
          name_map[ *sit ] = second;
	  Network::add( second );
	}
      }
      else {
        second = name_map[ *sit ];
      }
      //Add the edge:
      if( first && second ) {
	//the following line is the only different line from the readFrom fcn in network.cpp
	//simply changed Edge to Directed Edge
        add( DirectedEdge(first, second) );
	
      }
    }
  }
  DirectedNetwork::setOutNodes();
	
}

