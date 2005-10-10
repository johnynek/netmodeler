/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

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
    NodeIntStats ns(true);
    ns.collect(net, &Network::getDegree);
    const map<int, int>& deg_dist = ns.getDist();
    map<int, int>::const_reverse_iterator rit;
    rit = deg_dist.rbegin();
    int k_max = rit->first;
    double k_ave = ns.getAverage();
    double k2_ave = ns.getMoment2();
    cout << net->getNodeSize() << "\t"
	 << k_ave << "\t"
	 << k2_ave << "\t"
	 << k_max << endl;
    delete net;
  }

#endif
}
