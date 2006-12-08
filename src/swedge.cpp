/** 
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of Florida
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida
Copyright (C) 2005  Tae Woong Choi <twchoi@ufl.edu>, University of Florida

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

#include "swedge.h"
#include <iostream>
#include <sstream>

using namespace Starsky;
using namespace std;

SWEdge::SWEdge (RandAddrNode* start, RandAddrNode* end, const std::string& attr="") : _attr(attr), Edge(start, end)
{
}

std::string SWEdge::getAttributes() const
{
  //return _attr.c_str();  
  return _attr;  
}
std::string SWEdge:: printAttributes() const
{
  return _attr.c_str();
}

int SWEdge::getDistanceBetween(int node_size)
{
  
  return ( (RandAddrNode*)first)->getDistanceTo(node_size, (RandAddrNode*) second);
  
}

void SWEdge::printEdgeInfo() {
  cout << "(" << ((RandAddrNode*)first)->getAddress() << "," 
	  << ((RandAddrNode*)second)->getAddress() << "," << printAttributes() << ")" ;
}

