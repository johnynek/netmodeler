#include <netmodeler.h>
#include <cstdlib>
#include <cmath>
#include <map>

using namespace Starsky;
using namespace std;

int main(int argc, char* argv[])
{

  Ran1Random r(-1);
  int cons = 2;
  if( argc < 2 )
  {
    cerr << "Usage: " << argv[0] << " nodes" << endl;
    return 0;
  }
  int nodes = atoi(argv[1]);
  double p = (2.0)/((double)nodes);
#if 0
  MergeNetwork net(RandomNetwork(nodes, p, r), r, cons);

  double e_0 = (double)net.getEdges().size();
  double e_c = pow(nodes, 1.5) * sqrt((double)cons)/2.0;
  double t0 = -1.0 * e_c * atanh( e_0 / e_c )/((double)cons);
  double t_c = e_c/((double)cons) + t0;
  
  int t = 0;
  cout << "#e_c: " << e_c << endl
       << "#e_0: " << e_0 << endl
       << "#t_0: " << t0 << endl
       << "#t_c: " << t_c << endl
       << "#r: " << cons << endl;
  cout << "#time\tnodes\tedges" << endl;
  int total_time = (int)(3.0 * t_c);
  for(t = 0; t < total_time; t++) {
    cout << t << "\t"
	 << net.getNodes().size() << "\t"
	 << net.getEdges().size() << endl;
    net.incrementTime();
  }
#else

  //Here we are looking at the maximum degree as a function of N:
  for(int n = 8; n < nodes; n *= 2)
  {
    p = (2.0)/((double)n);
    MergeNetwork* net = new MergeNetwork(RandomNetwork(n, p, r), r, cons);
    net->incrementTime( (int)pow(n,1.5) );
    map<int, int> deg_dist = net->getDegreeDist();
    map<int, int>::const_reverse_iterator rit;
    rit = deg_dist.rbegin();
    int k_max = rit->first;
    double k_ave = net->getDegreeMoment(1);
    double k2_ave = net->getDegreeMoment(2);
    cout << net->getNodes().size() << "\t"
	 << k_ave << "\t"
	 << k2_ave << "\t"
	 << k_max << endl;
    delete net;
  }

#endif
}
