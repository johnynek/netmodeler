#include <netmodeler.h>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

  //Read in a network, compare the null partition, and the component partition:
  //
  NullPart np;
  ComponentPart cp;
  NetworkFactory nf;
  Network* n = nf.create(std::cin);
  set<Network*>* part0 = np.partition( *n );
  set<Network*>* part1 = cp.partition( *n );
  long n0, n1;
  long dist = cp.distance( part0, part1, n0, n1 );
  cout << "Norm0: " << n0 << endl << "Norm1: " << n1 << endl << "Dist: " << dist << endl;

}
