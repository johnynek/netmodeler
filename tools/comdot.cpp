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

#include <netmodeler.h>
#include <fstream>
#include <vector>
                                                                                  
using namespace std;
using namespace Starsky;

void printEdges(const Network& net,
		Network::EdgeSet& printed,
		const string& attributes)
{
  auto_ptr<EdgeIterator> eit( net.getEdgeIterator() );
  while( eit->moveNext() ) {
    Edge* this_edge = eit->current();
    if( printed.count( this_edge ) == 0 ) {
      Node* start = this_edge->first;
      Node* end = this_edge->second;
      cout << start->toString() << " -- " << end->toString()
	   << " " << attributes << ";" << endl;
    }
  }
  eit->reset();
  while( eit->moveNext() ) {
    printed.insert( eit->current() );
  }
}

void printCommunities(AgglomPart& ap, string prefix, const Network& net,
		      ostream& out, Network::EdgeSet& printed_edges,
		      int depth) {
    stringstream community;
    community << prefix << "_";
    //Print out the communities:
    vector< pair<int, int> > joins;
    vector<double> q_t;
    int step = ap.getCommunities(net, q_t, joins);
    out << "//" << prefix << "=" << q_t[step] << endl;
    out << "subgraph cluster" << prefix << " {" << endl;
    out << "node [style=filled];" << endl;
    if( q_t[step] > 0.25 ) {
      
      vector< Network* >* comms = ap.getCommunity(net,step, joins);
      vector< Network* >::const_iterator comit;
      Network::NodePSet::const_iterator comnodeit;
      int com = 0;
      for(comit = comms->begin();
	  comit != comms->end();
	  comit++) {
	Network* this_commun = *comit;
        stringstream this_com;
        this_com << community.str() << com++;
	//Recurse:
	printCommunities(ap, this_com.str(), *this_commun, out, printed_edges, depth + 1);
      }
      //Print the end of the subgraph
      
      //Free up the memory
      ap.deletePartition(comms);
    }
    else {
      //The community cannot be split further, so print it out:
      stringstream attributes;
      double length = (double)(4 >> depth);
      if(length <= 0.0) {
        length = 1.0;
      }
      attributes << "[len=" << length << ",";
      vector<string> colors;
      colors.push_back("black");
      colors.push_back("red");
      colors.push_back("blue");
      vector<string> styles;
      styles.push_back("bold");
      styles.push_back("dotted");
      styles.push_back("dashed");
      int style = (depth / 3) % 3;
      int color = depth % 3;
      attributes << "color=" << colors[ color ] << ",";
      attributes << "style=" << styles[ style ] << "]";
      printEdges(net, printed_edges, attributes.str());
    }
    out << "color = blue;" << endl
	<< "label = \"" << prefix << "\";" << endl;
    out << "}" << endl;
}

int main(int argc, char* argv) {

  Network my_net(cin);
  cout << "graph G {" << endl; 
  Network::EdgeSet printed_edges;
  //Look on components:
  ComponentPart cp;
  NewmanCom comfinder;
  vector<Network*>* components = cp.partition(my_net);
  vector<Network*>::const_iterator comp_it;
  int community = 0;
  for(comp_it = components->begin(); comp_it != components->end(); comp_it++) {
    stringstream com;
    com << community++;
    Network* this_net = *comp_it;
    printCommunities(comfinder, com.str(), *this_net, cout, printed_edges, 1);
  }
  cp.deletePartition(components);
  printEdges(my_net, printed_edges, "[len=16, color=green]");
  cout << "}" << endl;
  return 1;
}
