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
#include <cmath>

using namespace Starsky;
using namespace std;

int main(int argc, char* argv[])
{

  int size = atoi(argv[1]);
  Ran1Random r(-1);
  //PowerLawDRV pl(r, -2.0,2,(int)pow((double)size,0.5));
  //ConstDRV pl(4);
  UniformDRV pl(r, 2, 4);
  //PowerLawProbabilityFunction pl(-2.0,2,size);
  NetworkFactory* nf = new DegreeLawNetFac(size, pl, r, false);
  Network* net = nf->create();
  cout << "#Made network of node size: " << net->getNodeSize() << endl;
  cout << "#Made network of edge size: " << net->getEdgeSize() << endl;
  net->printTo(std::cout);
#if 0
  auto_ptr<NodeIterator> ni( net->getNodeIterator() );
  while( ni->moveNext() ) {
    cout << ni->current()->toString() << endl;
  }
  auto_ptr<EdgeIterator> ei( net->getEdgeIterator() );
  cout << "Got ei" << endl;
  while( ei->moveNext() ) {
    cout << ei->current()->toString() << endl;
  }
#endif
  delete nf;
  delete net;
}
