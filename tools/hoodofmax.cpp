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

/**
 * Print out the nodes within distance k of the maximum degree node
 */

#include <netmodeler.h>
#include <cstdlib>

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

  if( argc < 2 ) {
    cerr << "Usage: " << argv[0] << " distance";
    return 0;
  }
  
  int distance = atoi(argv[1]);
  Network net(cin);
  Network::NodePSet::iterator n_it;
  int max = -1;
  Node* max_n = 0;
  auto_ptr<NodeIterator> ni( net.getNodeIterator() );
  while( ni->moveNext() ) {
    Node* this_node = ni->current();
    if( net.getDegree(this_node) == 2 ) {
      max = net.getDegree(this_node);
      max_n = this_node;
      break;
    }
  }

  //Network* subgraph = net.getSubNet( net.getNeighborhood(max_n, distance) );
  ///\todo FIXME
  Network* subgraph = 0;
  subgraph->printTo(cout);
  delete subgraph;
  return 1;
}
