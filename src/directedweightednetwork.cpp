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

#include "directedweightednetwork.h"
#include "directednetwork.h"
#include "network.h"
#include "directededge.h"
#include "math.h"

using namespace Starsky;
using namespace std;

DirectedWeightedNetwork::DirectedWeightedNetwork() : DirectedNetwork() { }

DirectedWeightedNetwork::DirectedWeightedNetwork(std::istream& in){
	DirectedNetwork::readFrom(in);
}



bool DirectedWeightedNetwork::add(Edge* aEdge) {

  DirectedEdge* d_edge = dynamic_cast<DirectedEdge*>(aEdge);
 
  if( d_edge == 0) {
    //This is not a directed edge:
	  return false;  
	}
  
	if( has(*d_edge) == true) {
     //We don't add edges twice, we will increment its weight
     //Note that Joe has added the following lines
      EdgeSet::iterator e_it;
      e_it = edge_set.find(d_edge);
     
      if(e_it != edge_set.end() ){
	      double curr_weight = (*e_it)->getWeight();
	      (*e_it)->setWeight(curr_weight + 1.0);
      }
      //end of Joe's modification	
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
  
	//Account for the edges and nodes
  pair< Network::EdgeSet::iterator, bool> result;
    
  result = edge_set.insert( d_edge );
  if( result.second == true ) {
    //This is a new edge:
    incrementEdgeRefCount( d_edge );
    if( node_set.insert(start).second ) {
      incrementNodeRefCount( start );
    }
    if( node_set.insert(end).second ) {
      incrementNodeRefCount( end );
    }
    connection_map[start].insert(end);
    _node_to_edges[start].insert( d_edge );
    in_connection_map[end].insert(start);
    _node_to_edges[end].insert( d_edge );
	}  
	return result.second;
}

double DirectedWeightedNetwork::getAverageWeight(Node* node) const{
  if(DirectedNetwork::getOutDegree(node) > 0)
    return DirectedWeightedNetwork::getOutStrength(node)/DirectedNetwork::getOutDegree(node);
}

map<int, double> DirectedWeightedNetwork::getOutDegtoAverageStrengthMap(const NodePSet& nodes) const {
  NodePSet::const_iterator i = nodes.begin();
  map<int, int> deg_dist = DirectedNetwork::getOutDegreeDist(nodes);
  map<int, double> degtotalstrength_map;
  double strength = 0;
  int deg = 0;
  
  while(i != nodes.end()) {
    deg = DirectedNetwork::getOutDegree(*i);
    strength = getOutStrength(*i);
    if( degtotalstrength_map.count(deg) == 1 ) {      
      degtotalstrength_map[deg] += strength;
    }
    else {
      degtotalstrength_map[deg] = strength;      
    }
    i++;
  }
  int size = deg_dist.size();
  int check_size = degtotalstrength_map.size();
  if(size != check_size)
    cout << "Error!" << endl;

  int deg_arr[size];
  int deg_num_arr[size];
  int counter = 0;
  map<int, int>::iterator deg_it;
  for(deg_it = deg_dist.begin(); deg_it != deg_dist.end(); deg_it++ ){
    deg_arr[counter] = deg_it->first;
    deg_num_arr[counter] = deg_it->second;
    counter++;
  }//watch out for indexing out of order here!

  double strength_arr[size];
  counter = 0; //reset counter to zero
  map<int, double>::iterator str_it;
  for(str_it = degtotalstrength_map.begin(); str_it != degtotalstrength_map.end(); str_it++ ){
    strength_arr[counter] = str_it->second;
    counter++;
  }

  counter = 0;  //reset counter to zero
  map<int, double> degaveragestrength_map;
  map<int, double>::iterator degree_it = degaveragestrength_map.begin();
  while(counter < size ){
          degaveragestrength_map[deg_arr[counter]] = strength_arr[counter]/(double)deg_num_arr[counter];
	  counter++;
	  }//in the middle of doing it  
  return degaveragestrength_map;

}

map<int, double> DirectedWeightedNetwork::getOutDegtoAverageStrengthMap() const {
  return getOutDegtoAverageStrengthMap(out_node_set);
}

double DirectedWeightedNetwork::getDirectedWeight(Node* start, Node* end) const{
        
  	//Make sure that these nodes point to one another:
		map<Node*, ConnectedNodePSet>::const_iterator it;
		it = connection_map.find(start);
		if( it == connection_map.end() || it->second.count(end) == 0) {
    	return 0.0;
		}
		it = in_connection_map.find(end);
		if( it == in_connection_map.end() || it->second.count(start) == 0) {
    	return 0.0;
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
			    return d_edge1->getWeight();
			  }
	    }
		}
		return 0.0;  
}

map<double, int> DirectedWeightedNetwork::getEdgeWeightDist(const EdgeSet& edges) const {

  EdgeSet::const_iterator i = edges.begin();
  map<double, int> edgeweight_dist;
  double rep = 0;
  
  while(i != edges.end()) {
    rep = (*i)->getWeight();
    if( edgeweight_dist.count(rep) == 1 ) {
      edgeweight_dist[rep] = edgeweight_dist[rep] + 1;
    }
    else {
      edgeweight_dist[rep] = 1;
    }
    i++;
  }
  return edgeweight_dist;
}

map<double, int> DirectedWeightedNetwork::getEdgeWeightDist() const {
  return getEdgeWeightDist( edge_set );
}

double DirectedWeightedNetwork::getInStrength(Node* node) const{

  EdgeSet es = Network::getEdges(node);
  double var = 0.0;
  EdgeSet::const_iterator i;
  for(i = es.begin(); i != es.end(); i++){
    DirectedEdge* d_edge = dynamic_cast<DirectedEdge*>(*i);
    //the following if statement makes sure that only an out edge is counted
    if( d_edge->pointsFirstToSecond() && d_edge->second == node || !d_edge->pointsFirstToSecond() && d_edge->first == node )
      var += (*i)->getWeight();
  }
  return var;
}

map<double, int> DirectedWeightedNetwork::getNodeOutStrengthDist(NodePSet& nodes){

  NodePSet::const_iterator i = nodes.begin();
  map<double, int> outstrength_dist;
  double strength = 0;
  
  while(i != nodes.end()) {
    strength = getOutStrength(*i);
    if( outstrength_dist.count(strength) == 1 ) {
      outstrength_dist[strength] = outstrength_dist[strength] + 1;
    }
    else {
      outstrength_dist[strength] = 1;
    }
    i++;
  }
    return outstrength_dist;
}

map<double, int> DirectedWeightedNetwork::getNodeOutStrengthDist(){
  return getNodeOutStrengthDist(out_node_set);
}

double DirectedWeightedNetwork::getOutDegAverageStrengthCoefficient(NodePSet& nodes){
  NodePSet::const_iterator i;
  int counter = 0;
  for(i = nodes.begin(); i != nodes.end(); i++) {
	 if(getOutDegree(*i) == 0 )
	   counter++;
  }
  if(counter > 0)
    cout << "WARNING!  This correlation will not be computed correctly due to zero outdegree nodes" << endl;

  double ave_k_out = getOutDegreeMoment(1, nodes);
  double ave_s_out = getOutStrengthMoment(1, nodes);
  double ave_soverk_out = getOutAverageStrengthMoment(1, nodes);

  double sec_k_out = getOutDegreeMoment(2, nodes);
  double sec_soverk_out = getOutAverageStrengthMoment(2, nodes);

  double numerator = ave_s_out - ave_soverk_out*ave_k_out;
  double denominator = sqrt( (sec_soverk_out - ave_soverk_out*ave_soverk_out)*(sec_k_out - ave_k_out*ave_k_out) );
  double coef;

  if(denominator != 0.0) 
     coef = numerator/denominator;
  else
    coef = 99,999;  //this should never happen!
  return coef;
}

double DirectedWeightedNetwork::getOutDegAverageStrengthCoefficient(){
  return getOutDegAverageStrengthCoefficient(out_node_set);
}

double DirectedWeightedNetwork::getOutDegStrengthCoefficient(NodePSet& nodes){

  NodePSet::const_iterator i;
  int counter = 0;
  for(i = nodes.begin(); i != nodes.end(); i++) {
	 if(getOutDegree(*i) == 0 )
	   counter++;
  }
  if(counter > 0)
    cout << "WARNING!  This correlation will not be computed correctly due to zero outdegree nodes" << endl;

  double cross_prod = 0.0; //cross_prod = <k_out*weight_out>

       for(i = nodes.begin(); i != nodes.end(); i++) {
          cross_prod += getOutStrength( *i )*(double)getOutDegree( *i);
      }
       cross_prod = cross_prod / (double)(nodes.size());

  double ave_k_out = getOutDegreeMoment(1, nodes);
  double ave_w_out = getOutStrengthMoment(1, nodes);

  double sec_k_out = getOutDegreeMoment(2, nodes);
  double sec_w_out = getOutStrengthMoment(2, nodes);

  double numerator = cross_prod - ave_k_out*ave_w_out;
  double denominator = sqrt( (sec_k_out - ave_k_out*ave_k_out)*(sec_w_out - ave_w_out*ave_w_out) );
  double coef;

  if(denominator != 0.0) 
     coef = numerator/denominator;
  else
    coef = 10,000;  //this should never happen!
  return coef;
}



double DirectedWeightedNetwork::getOutDegStrengthCoefficient(){
  return getOutDegStrengthCoefficient(out_node_set);
}

double DirectedWeightedNetwork::getOutStrength(Node* node) const{

  EdgeSet es = Network::getEdges(node);
  double var = 0.0;
  EdgeSet::const_iterator i;
  for(i = es.begin(); i != es.end(); i++){
    DirectedEdge* d_edge = dynamic_cast<DirectedEdge*>(*i);
    //the following if statement makes sure that only an out edge is counted
    if( d_edge->pointsFirstToSecond() && d_edge->first == node || !d_edge->pointsFirstToSecond() && d_edge->second == node )
      var += (*i)->getWeight();
  }
  return var;
}

double DirectedWeightedNetwork::getOutStrengthMoment(int m) const{
  getOutStrengthMoment(m, out_node_set);
}

double DirectedWeightedNetwork::getOutStrengthMoment(int m, const NodePSet& nodes) const{
    double ave = 0.0;
    NodePSet::const_iterator i;
    if( m == 1) {
      for(i = nodes.begin(); i != nodes.end(); i++) {
          ave += getOutStrength( *i );
      }
    }
    else if( m == 2) {
      for(i = nodes.begin(); i != nodes.end(); i++) {
          ave += (getOutStrength( *i ) * getOutStrength( *i ));
      }	
    }
    else {
      for(i = nodes.begin(); i != nodes.end(); i++) {
          ave += pow( getOutStrength( *i ), m);
      }
    }
    return ave / (double)( nodes.size() );
}    

double DirectedWeightedNetwork::getOutAverageStrengthMoment(int m) const{
  getOutAverageStrengthMoment(m, out_node_set);
}

double DirectedWeightedNetwork::getOutAverageStrengthMoment(int m, const NodePSet& nodes) const{
    double sum = 0.0;
    int counter = 0;
    double temp;
    NodePSet::const_iterator i;
    if( m == 1) {
      for(i = nodes.begin(); i != nodes.end(); i++) {
	if(getOutDegree( *i ) >=1){
	  sum += getOutStrength( *i )/getOutDegree( *i );
	  counter++;
	}
      }
    }
    else if( m == 2) {
      for(i = nodes.begin(); i != nodes.end(); i++) {
        if(getOutDegree( *i ) >=1){
	  temp = getOutStrength( *i )/getOutDegree( *i );
	  sum += temp*temp;
	  counter++;
	}
      }	
    }
    else {
      for(i = nodes.begin(); i != nodes.end(); i++) {
        if(getOutDegree( *i ) >=1){
	  temp = getOutStrength( *i )/getOutDegree( *i );
	  sum += pow( temp, m);
	  counter++;
	}
      }
    }
    return sum / (double)( counter );
}  


void DirectedWeightedNetwork::printOutDegStrengthPlot(NodePSet& nodes, ostream& out){
  NodePSet::const_iterator i;
       for(i = nodes.begin(); i != nodes.end(); i++) {
	 out << (double)getOutDegree( *i) << " " << getOutStrength( *i ) << endl;
      }
}

void DirectedWeightedNetwork::printOutDegStrengthPlot(ostream& out){
  printOutDegStrengthPlot(out_node_set, out);
}

void DirectedWeightedNetwork::printInZeros(ostream& out) const{
    out << "#The following nodes have indegree zero" << endl;
    NodePSet::const_iterator i;
    for(i = node_set.begin(); i != node_set.end(); i++){
	    if(getInDegree(*i) == 0){
		    out << (*i)->toString() << endl;
	    }
    }
}

void DirectedWeightedNetwork::printOutZeros(ostream& out) const{
    out << "#The following nodes have outdegree zero" << endl;
    NodePSet::const_iterator i;
    for(i = node_set.begin(); i != node_set.end(); i++){
	    if(getOutDegree(*i) == 0){
		    out << (*i)->toString() << endl;
	    }
    }

}

void DirectedWeightedNetwork::printOutStrengthDist(std::ostream& out){
	//Print out the strength distribution:
	map<double, int> strength_dist = DirectedWeightedNetwork::getNodeOutStrengthDist();
        map<double, int>::iterator strength_it = strength_dist.begin();
	out << "#printing out strength distribution" << endl;
	out << "Strength Number" << endl;
	for(;strength_it != strength_dist.end(); strength_it++ ) {
          out << strength_it->first << " " << strength_it->second << endl;
	}
}

void DirectedWeightedNetwork::printOutDegStrengthMap(std::ostream& out){
	//Print out the map:
	map<int, double> my_map = DirectedWeightedNetwork::getOutDegtoAverageStrengthMap();
        map<int, double>::iterator deg_it;
	out << "#printing out degree to average strength map" << endl;
	out << "Node_degree Average_Strength" << endl;
	for(deg_it = my_map.begin(); deg_it != my_map.end(); deg_it++ ) {
          out << deg_it->first << " " << deg_it->second << endl;
	}
}

void DirectedWeightedNetwork::printWeights(std::ostream& out) const{
    EdgeSet::const_iterator i;
    for(i = edge_set.begin(); i != edge_set.end(); i++){
	    out << (*i)->toString() << " with weight = " << (*i)->getWeight() << endl;
    }
}


