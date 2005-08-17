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
