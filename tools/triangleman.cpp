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

int main(int argc, char* argv[])
{
  Ran1Random r(-1);
#if 1
  //Incrementable networks
  //UniformAttachNet my_net(RandomNetwork(7, 0.5, r), r, 2);
  PrefAtNetwork my_net(RandomNetwork(3,1,r),r,2);
	       
  int triangles, wedges;
  cout << "#size triangles wedges" << endl;
  while(my_net.getNodes().size() < 5000) {
    my_net.incrementTime(100);
    my_net.getTrianglesWedges(triangles, wedges);
    cout << my_net.getNodes().size() << " " << triangles << " " << wedges << endl;
  }
#else
  //Static networks:
  cout << "#size triangles wedges" << endl;
  int triangles, wedges;
  for(int i = 100; i < 5000; i+=100) {
    ExpProbabilityFunction expl(2.0/3.0,2, i);
    DegreeLawRandomNetwork my_net(i,expl,r,true);
    my_net.getTrianglesWedges(triangles, wedges);
    cout << my_net.getNodes().size() << " " << triangles << " " << wedges << endl;
  }
#endif
}
