/**
 * This code tests bond percolation on networks at various sizes
 * near the threshold
 */

#include <netmodeler.h>
#include <algorithm>

#include <cstdlib>

using namespace std;
using namespace Starsky;


int main(int argc, char* argv[])
{
  if( argc < 6 ) {
   cerr << "Usage: " << argv[0] << " nodes exp eps count uniform" << endl;
   return 1;
  }
  int max_nodes = atoi(argv[1]);
  double exp = atof(argv[2]);
  double eps = atof(argv[3]);
  int count = atoi(argv[4]);
  bool uniform_p = (bool)atoi(argv[5]);
  
  cout << "#max_nodes: " << max_nodes << endl
       << "#exp: " << exp << endl
       << "#eps: " << eps << endl
       << "#count: " << count << endl
       << "#uniform_p: " << uniform_p << endl;
  //////////////////////////////////////////////////
  cout << "#nodes n_largest perc_edges e_largest pre_perc_edges" << endl;
  Ran1Random ran;
  int nodes = 1000;
  int k_max = (int)sqrt(sqrt((double)5000));
  //PowerLawProbabilityFunction pl(-1.0*exp,2,k_max);
  for(int steps = 0; nodes < max_nodes; steps++) {
    nodes = (int)(nodes * 1.05);
    //Build the network
    int size = 0;
    int edges = 0;
    int edges_comp = 0;
    PowerLawProbabilityFunction pl(-1.0*exp,2,(int)pow((double)nodes,1.0/exp));
    DegreeLawRandomNetwork net(nodes, pl, ran, uniform_p);
    //Get the percolation threshold:
    double k1 = net.getDegreeMoment(1);
    double k2 = net.getDegreeMoment(2);
    double q_c = k1/(k2 - k1);
    double q = q_c * (1.0 + eps);
   
    //Here is a bond percolator:
    INetworkMapper* bp = new PercolationMapper(ran, q);
    
    int pre_perc_edges = net.getEdges().size();
   
    //This is an INetworkPartioner that just
    //looks for all the components
    ComponentPart cp;
    for(int k = 0; k < count; k++) {
   
      Network net2 = net;
      bp->map(&net2);
      //Here are the number of edges that remain after percolation
      edges += net2.getEdges().size();
      //Get the components:
      set<Network*>* comps = cp.partition(net2);
      //Here are the number of nodes in the largest component
      set<Network*>::iterator cit = max_element(comps->begin(), comps->end(),
		                                      networkptr_lt());
      size += (*cit)->getNodes().size();
      //Here are the number of edges in the largest component.
      edges_comp += (*cit)->getEdges().size();
      cp.deletePartition(comps);
    }
    
    double ave_size = (double)size/(double)count;
    double ave_edge = (double)edges/(double)count;
    double ave_edge_comp = (double)edges_comp/(double)count;
    cout << nodes << "\t" << ave_size << "\t"
	 << ave_edge << "\t" << ave_edge_comp
	 << "\t" << pre_perc_edges << endl;
  }
  return 0;
}
