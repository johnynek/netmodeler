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

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {
	
	//Make a network:
	Ran1Random my_rand, my_rand2(-100), my_rand3(-1000);
	Network my_net(cin);
        NodeIntStats ns(true);
	ns.collect(&my_net, &Network::getDegree);
	ns.collectByEdge(&my_net, &Network::getDegree);
	double h1, h2, h3;
	ns.getEdgeEntropy(h1,h2,h3);
        cout << "#loaded network" << endl;	
	cout << "#assortativity: " << ns.getEdgeCorrelation() << endl;
	cout << "#cluster coeff: " << my_net.getClusterCoefficient() << endl;
	cout << "#edgecc: " << my_net.getEdgeCC() << endl;
	cout << "#transitivity: " << my_net.getTransitivity() << endl;
	cout << "#nodes: " << my_net.getNodeSize() << endl;
	cout	     << "#edges: " << my_net.getEdgeSize() << endl;
	cout	     << "#<k>: " << ns.getAverage() << endl;
	cout	     << "#<k^2>: " << ns.getMoment(2.0) << endl;
	cout	     << "#H(degs): " << ns.getEntropy() << endl;
	cout	     << "#H(e_i): " << h1 << endl;
	cout	     << "#H(e_ij): " << h3 << endl;
	cout	     << "#EdgeMI: " << ns.getEdgeMutualInfo() << endl;
	
	//Print out the degree distribution:
	const map<int, int>& deg_dist = ns.getDist();
        map<int, int>::const_iterator deg_it = deg_dist.begin();
	cout << "#printing out degree distribution" << endl;
	for(;deg_it != deg_dist.end(); deg_it++ ) {
          cout << deg_it->first << " " << deg_it->second << endl;
	}
  return 1;
}
