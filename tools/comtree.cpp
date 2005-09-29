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
#include <fstream>
                                                                                  
using namespace std;
using namespace Starsky;

int main(int argc, char* argv) {

  Network my_net(cin);
  vector< pair<int, int> > joins;
  vector<double> q_t;
  NewmanCom comfinder;
  comfinder.getCommunities(my_net,q_t, joins);
  //In order to know which community refers to what, we must build a table:
  vector<Node*> all_nodes;
  NodeIterator ni = my_net.getNodeIterator();
  while( ni.moveNext() ) {
    all_nodes.push_back( ni.current() );
  }
  for(int i = 0; i < joins.size(); i++) {
      int join1, join2;
      join1 = joins[i].first;
      join2 = joins[i].second;
      cout << join1 << "\t" << join2 << "\t" << q_t[i] << endl;
  }
  cout << "#<tab>id<tab>node:" << endl;
  for(int i = 0; i < all_nodes.size(); i++) {
    cout << "#\t" << i << "\t" << all_nodes[i]->toString() << endl;
  }
#if 0
  //Look on components:
  set<Network> components = my_net.getComponents();
  set<Network>::const_reverse_iterator comp_it;
  int community = 0;
  for(comp_it = components.rbegin(); comp_it != components.rend(); comp_it++) {
    comp_it->getCommunities(q_t, joins);
    for(int i = 0; i < joins.size(); i++) {
      cout << joins[i].first << "\t" << joins[i].second << "\t"
	   << q_t[i] << endl;
    }
  }
#endif
  return 1;
}
