#include <netmodeler.h>
#include <fstream>
#include <vector>
                                                                                  
using namespace std;
using namespace Starsky;

void printEdges(const Network::EdgeSet& print,
		Network::EdgeSet& printed,
		const string& attributes)
{
  Network::EdgeSet::const_iterator e_it;
  for(e_it = print.begin(); e_it != print.end(); e_it++) {
    if( printed.count( *e_it ) == 0 ) {
      Node* start = (*e_it)->first;
      Node* end = (*e_it)->second;
      cout << start->toString() << " -- " << end->toString()
	   << " " << attributes << ";" << endl;
    }
  }
  printed.insert(print.begin(), print.end());
}

void printCommunities(string prefix, const Network& net,
		      ostream& out, Network::EdgeSet& printed_edges,
		      int depth) {
    stringstream community;
    community << prefix << "_";
    //Print out the communities:
    vector< pair<int, int> > joins;
    vector<double> q_t;
    int step = net.getCommunities(q_t, joins);
    out << "//" << prefix << "=" << q_t[step] << endl;
    out << "subgraph cluster" << prefix << " {" << endl;
    out << "node [style=filled];" << endl;
    if( q_t[step] > 0.25 ) {
      
      set< Network > comms = net.getCommunity(step, joins);
      set< Network >::const_reverse_iterator comit;
      Network::NodePSet::const_iterator comnodeit;
      int com = 0;
      for(comit = comms.rbegin();
	  comit != comms.rend();
	  comit++) {
        stringstream this_com;
        this_com << community.str() << com++;
	//Recurse:
	printCommunities(this_com.str(), *comit, out, printed_edges, depth + 1);
      }
      //Print the end of the subgraph
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
      printEdges(net.getEdges(), printed_edges, attributes.str());
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
  set<Network> components = my_net.getComponents();
  set<Network>::const_reverse_iterator comp_it;
  int community = 0;
  for(comp_it = components.rbegin(); comp_it != components.rend(); comp_it++) {
    stringstream com;
    com << community++;
    printCommunities(com.str(), *comp_it, cout, printed_edges, 1);
  }
  printEdges(my_net.getEdges(), printed_edges, "[len=16, color=green]");
  cout << "}" << endl;
  return 1;
}
