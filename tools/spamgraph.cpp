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
       for(n_it = this_net->getNodes().begin();
	   n_it != this_net->getNodes().end();
	   n_it++) {
         if( kmax < this_net->getDegree( *n_it ) ) {
           kmax = this_net->getDegree( *n_it );
	 }
       }
       int size = this_net->getNodes().size();
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
    for(nit = (*comp_it)->getNodes().begin();
	nit != (*comp_it)->getNodes().end();
	nit++) {
      spam << (*nit)->toString() << endl;
    }
  }
  for(comp_it = hamg.begin(); comp_it != hamg.end(); comp_it++) {
    for(nit = (*comp_it)->getNodes().begin();
	nit != (*comp_it)->getNodes().end();
	nit++) {
      ham << (*nit)->toString() << endl;
    }
  }
  //Delete the memory:
  return 1;
}
