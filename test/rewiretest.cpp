#include <netmodeler.h>

using namespace Starsky;
using namespace std;

int main(int argc, char* argv[])
{
  //Make some network: 
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

  EdgeFactory ef;

  EdgeRewirer mapper(ef, 0.75, ran);
  mapper.map(&my_net);
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

  return 0;
}
