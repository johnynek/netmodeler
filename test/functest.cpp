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
#include <iostream>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[])
{
  double exp = 2.0;
  int nodes = 1000;
  bool uniform_p = true;
  Ran1Random ran;
  PowerLawDRV pl(ran, -1.0*exp,2,(int)pow((double)nodes,1.0/exp));
  DegreeLawNetFac my_fac(nodes, pl, ran, uniform_p);
  auto_ptr<Network> my_net( my_fac.create() );

        cout << "#loaded network" << endl;
        cout << "#assortativity: " << my_net->getAssortativity() << endl
             << "#cluster coeff: " << my_net->getClusterCoefficient() << endl
             << "#transitivity: " << my_net->getTransitivity() << endl
             << "#nodes: " << my_net->getNodeSize() << endl
             << "#edges: " << my_net->getEdgeSize() << endl
             << "#<k>: " << my_net->getDegreeMoment(1) << endl
             << "#<k^2>: " << my_net->getDegreeMoment(2) << endl
             << "#H(degs): " << my_net->getDegreeEntropy() << endl
             << "#H(e_i): " << my_net->getEdgeEntropy().first << endl
             << "#H(e_ij): " << my_net->getEdgeEntropy().second << endl
             << "#EdgeMI: " << my_net->getEdgeMutualInfo() << endl;

#ifndef HIDE_STL
  node_deg deg = my_net->forEachNode( node_deg(my_net) );
  cout << "sum: " << deg.sum/2 << " calls: " << deg.calls << endl;
  node_i_func out = my_net->forEachNode(
		  node_i_func(my_net, &Network::getDegree) );
  node_d_func d2 = my_net->forEachNode(
		  node_d_func(my_net, &Network::getDegree, 2.0) );
  cout << "<k^2>: " << d2.average() << endl;
  node_d_func cc = my_net->forEachNode(
                  node_d_func(my_net, &Network::getClusterCoefficient)
		  );
  cout << "sum : " << out.sum <<  endl;
  cout << "calls: " << out.calls << endl;
  cout << "cc: " << cc.average() << endl;
#endif
}
