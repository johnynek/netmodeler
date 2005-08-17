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

/*  int EmailNetwork::getInternalSize(NodePSet& nodes){
    int counter = 0;
    NodePSet::const_iterator i;
     for(i = nodes.begin(); i != nodes.end(); i++) {
       std::string str = (*i)->toString();
	 if(str[0] == 's')
          counter += 1;	 
      }
     return counter;
     }*/
  
  int EmailNetwork::getInternalSize(){
    return internal_node_set.size();
  }

int EmailNetwork::getInternalOutSize(){
  return internal_out_node_set.size();
}

map<int, double> EmailNetwork::getInternalOutDegtoAverageStrengthMap() const{
  return DirectedWeightedNetwork::getOutDegtoAverageStrengthMap(internal_out_node_set);
}

double EmailNetwork::getInternalOutDegStrengthCoefficient(){
  return DirectedWeightedNetwork::getOutDegStrengthCoefficient(internal_out_node_set);
}

double EmailNetwork::getInternalOutDegAverageStrengthCoefficient(){
  return DirectedWeightedNetwork::getOutDegAverageStrengthCoefficient(internal_out_node_set);
}

map<double, int> EmailNetwork::getInternalNodeOutStrengthDist(){
  return getNodeOutStrengthDist(internal_out_node_set);
}

void EmailNetwork::removeSmallComponents(){

     set<DirectedNetwork> components = this->DirectedNetwork::getUndirectedComponents();
     set<DirectedNetwork>::iterator comp_it;

     int size = 0;
     int max_size = 0;

     for( comp_it = components.begin(); comp_it != components.end(); comp_it++ ){
	     size = comp_it->getNodes().size();
	     if(size > max_size){
		     max_size = size;
	     }
     }
     

     for( comp_it = components.begin(); comp_it != components.end(); comp_it++ ){
	     size = comp_it->getNodes().size();
	     if(size < max_size){
	       this->Network::remove( (*comp_it).getNodes() );
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
  Network::NodePSet nodes = this->getNodes();
  Network::NodePSet::const_iterator i;
  for(i = nodes.begin(); i != nodes.end(); i++) {
    if(this->getOutStrength(*i) == 0.0 )
      this->remove(*i);
  }
  
}

void EmailNetwork::setInternalNodes(NodePSet& nodes) {
    NodePSet::const_iterator i;
     for(i = nodes.begin(); i != nodes.end(); i++) {
       std::string str = (*i)->toString();
	 if(str[0] == 's')
          internal_node_set.insert(*i);	 
      }     
}

void EmailNetwork::setInternalNodes() {
    setInternalNodes(node_set);
}

void EmailNetwork::setInternalOutNodes(NodePSet& nodes) {
    NodePSet::const_iterator i;
     for(i = nodes.begin(); i != nodes.end(); i++) {
       std::string str = (*i)->toString();
	 if(str[0] == 's' && getOutDegree(*i) >= 1)
          internal_out_node_set.insert(*i);	 
      }     
}

void EmailNetwork::setInternalOutNodes() {
    setInternalOutNodes(node_set);
}
