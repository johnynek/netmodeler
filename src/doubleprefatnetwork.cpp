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

#include "doubleprefatnetwork.h"

using namespace Starsky;
using namespace std;

DoublePrefAtNetwork::DoublePrefAtNetwork(const Network& seed,
			                 Random& r,
			                 int c) : PrefAtNetwork(seed,r,c)
{
}
										 

DoublePrefAtNetwork::DoublePrefAtNetwork(const Network& seed,
			                 Random& r,
					 const DegreePreferenceFunction& dp,
			                 int c) : PrefAtNetwork(seed,r,dp,c)
{
}

void DoublePrefAtNetwork::postNodeAdd(Node* node)
{
  //Select two nodes preferentially, and add an edge between them
  Node* first = findPartnerFor(0);
  Node* second = findPartnerFor(first);
  if( first && second ) {
    Network::add( Edge(first,second) );
  }
  if( first == second ) {
    cerr << "first == second" << endl;
  }
}
