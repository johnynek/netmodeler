#include <netmodeler.h>
#include <iostream>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[])
{
  double exp = 2.0;
  int nodes = 1000;
  bool uniform_p = true;
  Ran1Random ran;
  PowerLawProbabilityFunction pl(-1.0*exp,2,(int)pow((double)nodes,1.0/exp));
  DegreeLawRandomNetwork my_net(nodes, pl, ran, uniform_p);

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

  node_deg deg = my_net.forEachNode( node_deg(&my_net) );
  cout << "sum: " << deg.sum/2 << " calls: " << deg.calls << endl;
  node_i_func out = my_net.forEachNode(
		  node_i_func(&my_net, &Network::getDegree) );
  node_d_func d2 = my_net.forEachNode(
		  node_d_func(&my_net, &Network::getDegree, 2.0) );
  cout << "<k^2>: " << d2.average() << endl;
  node_d_func cc = my_net.forEachNode(
                  node_d_func(&my_net, &Network::getClusterCoefficient)
		  );
  cout << "sum : " << out.sum <<  endl;
  cout << "calls: " << out.calls << endl;
  cout << "cc: " << cc.average() << endl;
}
