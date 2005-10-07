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
#include "emailnetwork.h"
#include "directedweightednetwork.h"
#include <string>

using namespace Starsky;
using namespace std;

  EmailNetwork::EmailNetwork() : DirectedWeightedNetwork() { }

  EmailNetwork::EmailNetwork(std::istream& in){
	DirectedWeightedNetwork::readFrom(in);
	EmailNetwork::setInternalNodes();
	EmailNetwork::setInternalOutNodes();
  }

  int EmailNetwork::getInternalSize(){
    return internal_node_set.getNodeSize();
  }

int EmailNetwork::getInternalOutSize(){
  return internal_out_node_set.getNodeSize();
}

map<int, double> EmailNetwork::getInternalOutDegtoAverageStrengthMap() const{
  auto_ptr<NodeIterator> ni( internal_out_node_set.getNodeIterator() );
  return DirectedWeightedNetwork::getOutDegtoAverageStrengthMap(ni.get());
}

double EmailNetwork::getInternalOutDegStrengthCoefficient(){
  auto_ptr<NodeIterator> ni( internal_out_node_set.getNodeIterator() );
  return DirectedWeightedNetwork::getOutDegStrengthCoefficient(ni.get());
}

double EmailNetwork::getInternalOutDegAverageStrengthCoefficient(){
  auto_ptr<NodeIterator> ni( internal_out_node_set.getNodeIterator() );
  return DirectedWeightedNetwork::getOutDegAverageStrengthCoefficient(ni.get());
}

map<double, int> EmailNetwork::getInternalNodeOutStrengthDist(){
  auto_ptr<NodeIterator> ni( internal_out_node_set.getNodeIterator() );
  return getNodeOutStrengthDist( ni.get() );
}

void EmailNetwork::removeSmallComponents(){

#ifndef HIDE_STL
     set<DirectedNetwork> components = this->DirectedNetwork::getUndirectedComponents();
#else
     ///\todo FIXME
     set<DirectedNetwork> components;
#endif
     set<DirectedNetwork>::iterator comp_it;

     int size = 0;
     int max_size = 0;

     for( comp_it = components.begin(); comp_it != components.end(); comp_it++ ){
	     size = comp_it->getNodeSize();
	     if(size > max_size){
		     max_size = size;
	     }
     }
     

     for( comp_it = components.begin(); comp_it != components.end(); comp_it++ ){
	     size = comp_it->getNodeSize();
	     if(size < max_size){
               auto_ptr<NodeIterator> ni( comp_it->getNodeIterator() );
	       remove(ni.get());
	     }
     }
	    
}

void EmailNetwork::printInternalOutStrengthDist(std::ostream& out){
	//Print out the strength distribution:
	map<double, int> strength_dist = EmailNetwork::getInternalNodeOutStrengthDist();
        map<double, int>::iterator strength_it = strength_dist.begin();
	out << endl;
	out << "#printing out strength distribution (for only the internal nodes)" << endl;
	out << "Strength Number" << endl;
	for(;strength_it != strength_dist.end(); strength_it++ ) {
          out << strength_it->first << " " << strength_it->second << endl;
	}
}

void EmailNetwork::printOutDegStrengthMap(std::ostream& out){
	//Print out the map:
	map<int, double> my_map = EmailNetwork::getInternalOutDegtoAverageStrengthMap();
        map<int, double>::iterator deg_it;
	out << "#printing out INTERNAL degree to average strength map" << endl;
	out << "Node_degree Average_Strength" << endl;
	for(deg_it = my_map.begin(); deg_it != my_map.end(); deg_it++ ) {
          out << deg_it->first << " " << deg_it->second << endl;
	}
}

void EmailNetwork::removeSinkNodes()
{
  auto_ptr<NodeIterator> ni( this->getNodeIterator() );
  while( ni->moveNext() ) {
    Node* this_node = ni->current();
    if(this->getOutStrength(this_node) == 0.0 )
      this->remove(this_node);
  }
  
}

void EmailNetwork::setInternalNodes(NodeIterator* nodes) {
  while( nodes->moveNext() ) {
    Node* this_node = nodes->current();
    std::string str = this_node->toString();
    if(str[0] == 's')
      internal_node_set.add(this_node);	 
    }     
}

void EmailNetwork::setInternalNodes() {
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    setInternalNodes( ni.get() );
}

void EmailNetwork::setInternalOutNodes(NodeIterator* nodes) {
     while( nodes->moveNext() ) {
       Node* this_node = nodes->current();
       std::string str = this_node->toString();
	 if(str[0] == 's' && getOutDegree(this_node) >= 1)
          internal_out_node_set.add(this_node);	 
      }     
}

void EmailNetwork::setInternalOutNodes() {
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    setInternalOutNodes(ni.get());
}
