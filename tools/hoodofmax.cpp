/**
 * Print out the nodes within distance k of the maximum degree node
 */

#include <netmodeler.h>
#include <cstdlib>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

  if( argc < 2 ) {
    cerr << "Usage: " << argv[0] << " distance";
    return 0;
  }
  
  int distance = atoi(argv[1]);
  Network net(cin);
  Network::NodePSet::iterator n_it;
  int max = -1;
  Node* max_n = 0;
  for(n_it = net.getNodes().begin();
      n_it != net.getNodes().end();
      n_it++) {
    if( net.getDegree(*n_it) == 2 ) {
      max = net.getDegree(*n_it);
      max_n = *n_it;
      break;
    }
  }

  Network* subgraph = net.getSubNet( net.getNeighborhood(max_n, distance) );
  subgraph->printTo(cout);
  delete subgraph;
  return 1;
}
