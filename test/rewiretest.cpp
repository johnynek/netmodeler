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

using namespace Starsky;
using namespace std;

void printInfo(Network& my_net)
{
        cout << "#loaded network" << endl;
        cout << "#assortativity: " << my_net.getAssortativity() << endl
             << "#cluster coeff: " << my_net.getClusterCoefficient() << endl
             << "#transitivity: " << my_net.getTransitivity() << endl
             << "#nodes: " << my_net.getNodeSize() << endl
             << "#edges: " << my_net.getEdgeSize() << endl
             << "#<k>: " << my_net.getDegreeMoment(1) << endl
             << "#<k^2>: " << my_net.getDegreeMoment(2) << endl
             << "#H(degs): " << my_net.getDegreeEntropy() << endl
             << "#H(e_i): " << my_net.getEdgeEntropy().first << endl
             << "#H(e_ij): " << my_net.getEdgeEntropy().second << endl
             << "#EdgeMI: " << my_net.getEdgeMutualInfo() << endl;
}

int main(int argc, char* argv[])
{
  //Make some network: 
  double exp = 2.0;
  int nodes = 1000;
  bool uniform_p = true;
  Ran1Random ran;
  PowerLawProbabilityFunction pl(-1.0*exp,2,(int)pow((double)nodes,1.0/exp));
  //DegreeLawRandomNetwork my_net(nodes, pl, ran, uniform_p);
  CallawayNetwork my_net(10000, 0.5, ran);

  printInfo(my_net);

  EdgeFactory ef;

  //EdgeRewirer mapper(ef, 0.75, ran);
  EdgeSwapper mapper(ef,ran);
  cout << "about to map:"<< endl;
  mapper.map(&my_net);
  printInfo(my_net);
  return 0;
}
