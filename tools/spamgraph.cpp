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
  set<Network> spamg, hamg;
  
     set<Network> components = graph.getComponents();
     set<Network>::iterator comp_it;
     Edge* cut_edge = 0;
     while( components.size() > 0 ) {
       //Choose the biggest graph
       comp_it = components.begin();
       Network::NodePSet::const_iterator n_it;
       int kmax = 0;
       for(n_it = comp_it->getNodes().begin();
	   n_it != comp_it->getNodes().end();
	   n_it++) {
         if( kmax < comp_it->getDegree( *n_it ) ) {
           kmax = comp_it->getDegree( *n_it );
	 }
       }
       int size = comp_it->getNodes().size();
       if( size > min_size ) {
         if( comp_it->getClusterCoefficient() > min_cc ) {
	   //Ham:
	   hamg.insert( *comp_it );
	 }
	 //Maybe spam:
	 else if( (kmax + 1) < kmfrac * size ) { 
           if( comp_it->getClusterCoefficient() == 0.0 ) {
             //Spam:
             spamg.insert( *comp_it );
	   }
           else {
             //Ambiguous
             map<Edge*, double> bet;
             set<Network> split;
             cut_edge = comp_it->getEdgeBetweenness(bet);
             Network tmp = *comp_it;
             tmp.remove( cut_edge->first );
             tmp.remove( cut_edge->second );
             split = tmp.getComponents();
             components.insert( split.begin(), split.end() );
           }
         }
       }
       //cout << "graphs: " << components.size() << endl;
       components.erase( comp_it );
     }

  //Print out the nodes in each spamg and hamg
  Network::NodePSet::const_iterator nit;
  for(comp_it = spamg.begin(); comp_it != spamg.end(); comp_it++) {
    for(nit = comp_it->getNodes().begin();
	nit != comp_it->getNodes().end();
	nit++) {
      spam << (*nit)->toString() << endl;
    }
  }
  for(comp_it = hamg.begin(); comp_it != hamg.end(); comp_it++) {
    for(nit = comp_it->getNodes().begin();
	nit != comp_it->getNodes().end();
	nit++) {
      ham << (*nit)->toString() << endl;
    }
  }
  return 1;
}
