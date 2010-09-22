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
#include <sstream>                                                                                
using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

  //Here is the input file name:
  ifstream input(argv[1]);
  Network my_net(input);
 
  ComponentPart cp;
  auto_ptr<NetworkPartition> components(cp.partition(my_net));
  int component = 0;
  auto_ptr<Iterator<Network*> > comp_it(components->getComponents());
  while(comp_it->moveNext()) {
    stringstream name;
    name << argv[1] << "." << component;
    component++;
    ofstream output(name.str().c_str());
    Network* this_comp = comp_it->current();
    this_comp->printTo(output);
  }
  return 1;
}
