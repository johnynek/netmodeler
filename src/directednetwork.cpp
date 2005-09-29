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
	    DirectedEdge* d_edge = new DirectedEdge();
	    *d_edge = d_e;
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
  
  Node  *start, *end;

  if( d_edge->pointsFirstToSecond() ) {
    start = d_edge->first;
	  end = d_edge->second;
  }
  else {
    start = d_edge->second;
	  end = d_edge->first;
  }
  if( !has( start ) ) {
    //start is new:
    add( start );
  }
  if( !has( end ) ) {
    add( end );
  }
  _node_to_edges[start].insert( d_edge );
  _node_to_edges[end].insert( d_edge );
  incrementEdgeRefCount( d_edge );
  return true;
}

///\todo: fix the below!
void DirectedNetwork::moveIntoUndirectedNetwork(Network& net) {

  NodeIterator ni = getNodeIterator();
  while( ni.moveNext() ) {
    net.add( ni.current() );
  }
  EdgeIterator ei = getEdgeIterator();
  while( ei.moveNext() ) {
    Edge* this_edge = ei.current();
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
  EdgeIterator ei = getEdgeIterator();
  while(ei.moveNext()) {
    //We need "remaining degree" for this calculation
    d_edge = dynamic_cast<DirectedEdge*>( ei.current() );
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
   * r = \frac{(\sum_i j_i k_i) - M^{-1}(\sum_i j_i)(\sum_i k_i)}
   *          {\sqrt{[(\sum_i j_i^2) - M^{-1}(\sum_i j_i)^2]
   *                    [(\sum_i k_i^2) - M^{-1}(\sum_i k_i)^2]}
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
		const EdgeSet& e1 = _node_to_edges.find(start)->second;
		const EdgeSet& e2 = _node_to_edges.find(end)->second;
		for(eit1 = e1.begin(); eit1 != e1.end(); eit1++) {
			for( eit2 = e2.begin(); eit2 != e2.end(); eit2++) {
			  //the following three lines were crucial in find the getEdgePtr bug
			  DirectedEdge* d_edge1 = dynamic_cast<DirectedEdge*>(*eit1);
			  DirectedEdge* d_edge2 = dynamic_cast<DirectedEdge*>(*eit2);
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

map<int, int> DirectedNetwork::getInDegreeDist(const NodePSet& nodes) const {

  NodePSet::const_iterator i;
  map<int, int> deg_dist;
  int deg = 0;

  for(i = nodes.begin(); i != nodes.end(); i++) {
    deg = getInDegree( *i );
    deg_dist[deg] = deg_dist[deg] + 1;
  }
  return deg_dist;
}

map<int, int> DirectedNetwork::getInDegreeDist() const {
  NodePSet node_set;
  fillNodePSet(node_set);
  return getInDegreeDist(node_set);
}

int DirectedNetwork::getInDegree(Node* node) const {
  int in_deg = 0;
  map<Node*, EdgeSet>::const_iterator neit = _node_to_edges.find(node);
  if( neit != _node_to_edges.end() ) {
    //He have this node:
    EdgeSet::const_iterator eit;
    for(eit = neit->second.begin();
        eit != neit->second.end();
        eit++) {
      DirectedEdge* d_e = dynamic_cast<DirectedEdge*>(*eit);
      if( d_e->getEndNode() == node ) {
        //This is an in-degree;
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

map<int, int> DirectedNetwork::getOutDegreeDist(const NodePSet& nodes) const {
  NodePSet::const_iterator i;
  map<int, int> deg_dist;
  int deg = 0;

  for(i = nodes.begin(); i != nodes.end();i++) {
    deg = getOutDegree( *i );
    deg_dist[ deg ] = deg_dist[deg] + 1;
  }
  return deg_dist;
}

map<int, int> DirectedNetwork::getOutDegreeDist() const {
  NodePSet node_set;
  fillNodePSet(node_set);
  return getOutDegreeDist( node_set );
}

int DirectedNetwork::getOutDegree(Node* node) const {
  int in_deg = 0;
  map<Node*, EdgeSet>::const_iterator neit = _node_to_edges.find(node);
  if( neit != _node_to_edges.end() ) {
    //He have this node:
    EdgeSet::const_iterator eit;
    for(eit = neit->second.begin();
        eit != neit->second.end();
        eit++) {
      DirectedEdge* d_e = dynamic_cast<DirectedEdge*>(*eit);
      if( d_e->getStartNode() == node ) {
        //This is an in-degree;
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

double DirectedNetwork::getInDegreeMoment(int m) const
{
  NodePSet node_set;
  fillNodePSet(node_set);
  return getInDegreeMoment(m,node_set);
}

double DirectedNetwork::getInDegreeMoment(int m, const NodePSet& nodes) const
{
  double ave = 0.0;
  NodePSet::const_iterator i;
  if( m == 1) {
    for(i = nodes.begin(); i != nodes.end(); i++) {
      ave += (double)getInDegree( *i );
    }
  }
  else if( m == 2) {
    for(i = nodes.begin(); i != nodes.end(); i++) {
      ave += (double)(getInDegree( *i ) * getInDegree( *i ));
    }	
  }
  else {
    for(i = nodes.begin(); i != nodes.end(); i++) {
      ave += pow( (double)getInDegree( *i ), m);
    }
  }
  return ave / (double)( nodes.size() );
}

double DirectedNetwork::getOutDegreeMoment(int m) const
{
  NodePSet node_set;
  fillNodePSet(node_set);
  return getOutDegreeMoment(m,node_set);
}

double DirectedNetwork::getOutDegreeMoment(int m, const NodePSet& nodes) const
{
  double ave = 0.0;
  NodePSet::const_iterator i;
  if( m == 1) {
    for(i = nodes.begin(); i != nodes.end(); i++) {
      ave += (double)getOutDegree( *i );
    }
  }
  else if( m == 2) {
    for(i = nodes.begin(); i != nodes.end(); i++) {
      ave += (double)(getOutDegree( *i ) * getOutDegree( *i ));
    }	
  }
  else {
    for(i = nodes.begin(); i != nodes.end(); i++) {
      ave += pow( (double)getOutDegree( *i ), m);
    }
  }
  return ave / (double)( nodes.size() );
}

void DirectedNetwork::printTo(ostream& out) const {

  /*
   * This is for use with graphviz, an AT&T graph visualization package
   * I had to cast the add some text ("n") so as not to confuse graphviz.
   */
	
  NodePSet::const_iterator i;
  ConnectedNodePSet::const_iterator j;
  map< Node*, ConnectedNodePSet >::const_iterator k;
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

int DirectedNetwork::remove(const Edge& edge) {

  Edge* edge_to_del = getEdgePtr(edge);
  return remove(edge_to_del);
}

int DirectedNetwork::remove(Edge* edge_to_del) {

  if( has(edge_to_del) ) {
    DirectedEdge* d_e = dynamic_cast<DirectedEdge*>(edge_to_del);
    Node* start;
    Node* end;

    if( d_e->pointsFirstToSecond() ) {
      start = d_e->first;
      end = d_e->second;
    }
    else {
      start = d_e->second;
      end = d_e->first;
    }
    _node_to_edges[start].erase(d_e);
    _node_to_edges[end].erase(d_e);
    decrementEdgeRefCount(d_e);  
    return 1;
  }
  else {
    //We don't have this edge:
    return 0;
  }
}

int DirectedNetwork::remove(Node* node) {
  int deleted_edges = 0;
  //Handle the out connections:
  map<Node*, EdgeSet>::iterator neit = _node_to_edges.find(node);
  if( neit != _node_to_edges.end() ) {
    //This node really exists
    EdgeSet es = neit->second;
    EdgeSet::iterator eit;
    for( eit = es.begin(); eit != es.end(); eit++) {
      remove( *eit );
      deleted_edges++;
    }
    decrementNodeRefCount(node);
  }
  else {
    //Not in the network:
  }
  return deleted_edges;
}

void DirectedNetwork::reverseEdges() {

  DirectedEdge* d_e;
  EdgeIterator ei = getEdgeIterator();
  while( ei.moveNext() ) {
      d_e = dynamic_cast<DirectedEdge*>( ei.current() );
      d_e->reverse();
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
  fillNodePSet(node_set);
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
        add( first );
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
	  add( second );
	}
      }
      else {
        second = name_map[ *sit ];
      }
      //Add the edge:
      if( first && second ) {
	//the following line is the only different line from the readFrom fcn in network.cpp
	//simply changed Edge to Directed Edge
        add( DirectedEdge::DirectedEdge(first, second) );
	
      }
    }
  }
  DirectedNetwork::setOutNodes();
	
}

