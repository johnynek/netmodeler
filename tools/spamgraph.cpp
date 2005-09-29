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

/**
 * This is an implementation of the spamgraph algorithm for
 * building black/whitelists from email headers.
 */

#include <netmodeler.h>
#include <fstream>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

  if( argc < 4 ) {
    //
    cout << "Usage: " << argv[0] << " graph blacklist whitelist" << endl;
    return 0;
  }
  ifstream input(argv[1]);
  ofstream spam(argv[2]);
  ofstream ham(argv[3]);
  Network graph(input);
  
  double min_cc = 0.1;
  double kmfrac = 0.6;
  int min_size = 10;
  double whole_graph_c = graph.getTransitivity();
  //The subgraphs of spam and nonspam
  set<Network*> spamg, hamg;
  
     ComponentPart cp;
     set<Network*>* components = cp.partition(graph);
     set<Network*>::iterator comp_it;
     Edge* cut_edge = 0;
     while( components->size() > 0 ) {
       //Choose the biggest graph
       comp_it = components->begin();
       Network* this_net = *comp_it;
       Network::NodePSet::const_iterator n_it;
       int kmax = 0;
       NodeIterator ni = this_net->getNodeIterator();
       while( ni.moveNext() ) {
	 Node* this_node = ni.current();
         if( kmax < this_net->getDegree( this_node ) ) {
           kmax = this_net->getDegree( this_node );
	 }
       }
       int size = this_net->getNodeSize();
       if( size > min_size ) {
         if( this_net->getClusterCoefficient() > min_cc ) {
	   //Ham:
	   hamg.insert( this_net );
	 }
	 //Maybe spam:
	 else if( (kmax + 1) < kmfrac * size ) { 
           if( this_net->getClusterCoefficient() == 0.0 ) {
             //Spam:
             spamg.insert( this_net );
	   }
           else {
             //Ambiguous
             map<Edge*, double> bet;
             set<Network*>* split = 0;
	     Network* this_comp = *comp_it;
             cut_edge = this_comp->getEdgeBetweenness(bet);
             this_comp->remove( cut_edge->first );
             this_comp->remove( cut_edge->second );
	     
             split = cp.partition(*this_comp);
             components->insert( split->begin(), split->end() );
           }
         }
       }
       //cout << "graphs: " << components.size() << endl;
       components->erase( comp_it );
     }

  //Print out the nodes in each spamg and hamg
  Network::NodePSet::const_iterator nit;
  for(comp_it = spamg.begin(); comp_it != spamg.end(); comp_it++) {
    NodeIterator ni2 = (*comp_it)->getNodeIterator();
    while( ni2.moveNext() ) {
      Node* this_node = ni2.current();
      spam << this_node->toString() << endl;
    }
  }
  for(comp_it = hamg.begin(); comp_it != hamg.end(); comp_it++) {
    NodeIterator ni2 = (*comp_it)->getNodeIterator();
    while( ni2.moveNext() ) {
      Node* this_node = ni2.current();
      ham << this_node->toString() << endl;
    }
  }
  //Delete the memory:
  return 1;
}
