#include <netmodeler.h>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {
	
	//Make a network:
	Ran1Random my_rand, my_rand2(-100), my_rand3(-1000);
	Network my_net(cin);

        cout << "#loaded network" << endl;	
	cout << "#assortativity: " << my_net.getAssortativity() << endl
	     << "#cluster coeff: " << my_net.getClusterCoefficient() << endl
	     << "#transitivity: " << my_net.getTransitivity() << endl
	     << "#nodes: " << my_net.getNodes().size() << endl
	     << "#edges: " << my_net.getEdges().size() << endl
	     << "#<k>: " << my_net.getDegreeMoment(1) << endl
	     << "#<k^2>: " << my_net.getDegreeMoment(2) << endl
	     << "#H(degs): " << my_net.getDegreeEntropy() << endl
	     << "#H(e_i): " << my_net.getEdgeEntropy().first << endl
	     << "#H(e_ij): " << my_net.getEdgeEntropy().second << endl
	     << "#EdgeMI: " << my_net.getEdgeMutualInfo() << endl;
	
	//Print out the degree distribution:
	map<int, int> deg_dist = my_net.getDegreeDist();
        map<int, int>::iterator deg_it = deg_dist.begin();
	cout << "#printing out degree distribution" << endl;
	for(;deg_it != deg_dist.end(); deg_it++ ) {
          cout << deg_it->first << " " << deg_it->second << endl;
	}
  return 1;
}
