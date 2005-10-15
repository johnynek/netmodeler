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
  
  Edge* e = getEdge(d_edge->getStartNode(), d_edge->getEndNode());
  
  if( e != 0 ) {
    //We don't add edges twice, we will increment its weight
    //Note that Joe has added the following lines
    //Make a new edge with the new weight
    /**
     * @todo I think this can leak memory since the caller assumes
     * aEdge is being managed by the network, but it is not.  We
     * probably need some reference counting checks here
     */
    double new_weight = e->getWeight() + d_edge->getWeight();
    d_edge = new DirectedWeightedEdge( d_edge->getStartNode(),
		                       d_edge->getEndNode(),
				       new_weight );
    remove(e);
  }
  //Okay, now we are sure d_edge should be added
  return DirectedNetwork::add(d_edge);
}

double DirectedWeightedNetwork::getAverageWeight(Node* node) const{
  if(DirectedNetwork::getOutDegree(node) > 0)
    return DirectedWeightedNetwork::getOutStrength(node)/DirectedNetwork::getOutDegree(node);
}

map<int, double> DirectedWeightedNetwork::getOutDegtoAverageStrengthMap(NodeIterator* nodes) const {
  map<int, int> deg_dist = DirectedNetwork::getOutDegreeDist(nodes);
  map<int, double> degtotalstrength_map;
  double strength = 0;
  int deg = 0;
  
  while(nodes->moveNext() ) {
    Node* this_node = nodes->current();
    deg = DirectedNetwork::getOutDegree(this_node);
    strength = getOutStrength(this_node);
    if( degtotalstrength_map.count(deg) == 1 ) {      
      degtotalstrength_map[deg] += strength;
    }
    else {
      degtotalstrength_map[deg] = strength;      
    }
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
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  return getOutDegtoAverageStrengthMap( ni.get() );
}

double DirectedWeightedNetwork::getDirectedWeight(Node* start, Node* end) const{
  //Get the edge:
  Edge* edge = getEdge(start,end);
  if( edge != 0 ) {
    return edge->getWeight();
  }
  else {
    return 0.0;
  }
}

map<double, int> DirectedWeightedNetwork::getEdgeWeightDist(EdgeIterator* edges) const {

  map<double, int> edgeweight_dist;
  double rep = 0;
  
  while(edges->moveNext() ) {
    Edge* this_edge = edges->current();
    rep = this_edge->getWeight();
    if( edgeweight_dist.count(rep) == 1 ) {
      edgeweight_dist[rep] = edgeweight_dist[rep] + 1;
    }
    else {
      edgeweight_dist[rep] = 1;
    }
  }
  return edgeweight_dist;
}

map<double, int> DirectedWeightedNetwork::getEdgeWeightDist() const {
  auto_ptr<EdgeIterator> ei( getEdgeIterator() );
  return getEdgeWeightDist( ei.get() );
}

double DirectedWeightedNetwork::getInStrength(Node* node) const{

  double var = 0.0;
  auto_ptr<EdgeIterator> ei( getEdgeIterator(node) );
  while( ei->moveNext() ) {
    Edge* this_edge = ei->current();
    DirectedEdge* d_edge = dynamic_cast<DirectedEdge*>(this_edge);
    //the following if statement makes sure that only an out edge is counted
    if( d_edge->pointsFirstToSecond() && d_edge->second == node || !d_edge->pointsFirstToSecond() && d_edge->first == node )
      var += (d_edge)->getWeight();
  }
  return var;
}

map<double, int> DirectedWeightedNetwork::getNodeOutStrengthDist(NodeIterator* nodes){

  map<double, int> outstrength_dist;
  double strength = 0;
  
  while( nodes->moveNext() ) {
    Node* this_node = nodes->current();
    strength = getOutStrength( this_node );
    if( outstrength_dist.count(strength) == 1 ) {
      outstrength_dist[strength] = outstrength_dist[strength] + 1;
    }
    else {
      outstrength_dist[strength] = 1;
    }
  }
    return outstrength_dist;
}

map<double, int> DirectedWeightedNetwork::getNodeOutStrengthDist(){
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  return getNodeOutStrengthDist( ni.get() );
}

double DirectedWeightedNetwork::getOutDegAverageStrengthCoefficient(NodeIterator* nodes){
  int counter = 0;
  while( nodes->moveNext() ) {
    Node* this_node = nodes->current();
	 if(getOutDegree(this_node) == 0 )
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
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  return getOutDegAverageStrengthCoefficient( ni.get() );
}

double DirectedWeightedNetwork::getOutDegStrengthCoefficient(NodeIterator* nodes){

  int counter = 0;
  while( nodes->moveNext() ) {
    Node* this_node = nodes->current();
	 if(getOutDegree(this_node) == 0 )
	   counter++;
  }
  if(counter > 0)
    cout << "WARNING!  This correlation will not be computed correctly due to zero outdegree nodes" << endl;

  double cross_prod = 0.0; //cross_prod = <k_out*weight_out>
  nodes->reset();
  int tot = 0;
  while( nodes->moveNext() ) {
    Node* this_node = nodes->current();
    cross_prod += getOutStrength( this_node )*(double)getOutDegree( this_node);
    tot++;
  }
  cross_prod = cross_prod / (double)(tot);

  
  nodes->reset();
  double ave_k_out = getOutDegreeMoment(1, nodes);
  nodes->reset();
  double ave_w_out = getOutStrengthMoment(1, nodes);

  nodes->reset();
  double sec_k_out = getOutDegreeMoment(2, nodes);
  nodes->reset();
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
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  return getOutDegStrengthCoefficient( ni.get() );
}

double DirectedWeightedNetwork::getOutStrength(Node* node) const{

  double var = 0.0;
  auto_ptr<EdgeIterator> ei( getEdgeIterator(node) );
  while( ei->moveNext() ) {
    DirectedEdge* d_edge = dynamic_cast<DirectedEdge*>(ei->current());
    //the following if statement makes sure that only an out edge is counted
    if( d_edge->pointsFirstToSecond() && d_edge->first == node || !d_edge->pointsFirstToSecond() && d_edge->second == node )
      var += d_edge->getWeight();
  }
  return var;
}

double DirectedWeightedNetwork::getOutStrengthMoment(int m) const{
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  getOutStrengthMoment(m, ni.get() );
}

double DirectedWeightedNetwork::getOutStrengthMoment(int m, NodeIterator* nodes) const{
    double ave = 0.0;
    int tot = 0; 
    if( m == 1) {
      while( nodes->moveNext() ) {
          Node* this_node = nodes->current();
          tot++;
          ave += getOutStrength( this_node );
      }
    }
    else if( m == 2) {
      while( nodes->moveNext() ) {
          Node* this_node = nodes->current();
          tot++;
          ave += (getOutStrength( this_node ) * getOutStrength( this_node ));
      }	
    }
    else {
      while( nodes->moveNext() ) {
          Node* this_node = nodes->current();
          tot++;
          ave += pow( getOutStrength( this_node ), m);
      }
    }
    return ave / (double)( tot );
}    

double DirectedWeightedNetwork::getOutAverageStrengthMoment(int m) const{
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  getOutAverageStrengthMoment(m, ni.get());
}

double DirectedWeightedNetwork::getOutAverageStrengthMoment(int m, NodeIterator* nodes) const{
    double sum = 0.0;
    int counter = 0;
    double temp;
    if( m == 1) {
      while( nodes->moveNext() ) {
        Node* this_node = nodes->current();
	if(getOutDegree( this_node ) >=1){
	  sum += getOutStrength( this_node )/getOutDegree( this_node );
	  counter++;
	}
      }
    }
    else if( m == 2) {
      while( nodes->moveNext() ) {
        Node* this_node = nodes->current();
        if(getOutDegree( this_node ) >=1){
	  temp = getOutStrength( this_node )/getOutDegree( this_node );
	  sum += temp*temp;
	  counter++;
	}
      }	
    }
    else {
      while( nodes->moveNext() ) {
        Node* this_node = nodes->current();
        if(getOutDegree( this_node ) >=1){
	  temp = getOutStrength( this_node )/getOutDegree( this_node );
	  sum += pow( temp, m);
	  counter++;
	}
      }
    }
    return sum / (double)( counter );
}  


void DirectedWeightedNetwork::printOutDegStrengthPlot(NodeIterator* ni, ostream& out){
  while( ni->moveNext() ) {
    Node* this_node = ni->current();
    out << (double)getOutDegree( this_node ) << " "
        << getOutStrength( this_node ) << endl;
  }
}

void DirectedWeightedNetwork::printOutDegStrengthPlot(ostream& out){
  auto_ptr<NodeIterator> ni( getNodeIterator() );
  printOutDegStrengthPlot( ni.get() , out);
}

void DirectedWeightedNetwork::printInZeros(ostream& out) const{
    out << "#The following nodes have indegree zero" << endl;
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while( ni->moveNext() ) {
      Node* this_node = ni->current();
      if(getInDegree(this_node) == 0) {
        out << this_node->toString() << endl;
      }
    }
}

void DirectedWeightedNetwork::printOutZeros(ostream& out) const{
    out << "#The following nodes have outdegree zero" << endl;
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while( ni->moveNext() ) {
      Node* this_node = ni->current();
      if(getOutDegree(this_node) == 0){
        out << (this_node)->toString() << endl;
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
    auto_ptr<EdgeIterator> ei( getEdgeIterator() );
    while( ei->moveNext() ) {
      Edge* this_edge = ei->current();
      out << this_edge->toString() << " with weight = " << this_edge->getWeight() << endl;
    }
}


