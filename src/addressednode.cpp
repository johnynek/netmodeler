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

#include "addressednode.h"

using namespace Starsky;

#define AMAX 65536
#define WMAX 4294967295

AddressedNode::AddressedNode()
{
  _c_address=0;
  _q_address=0;
}


AddressedNode::AddressedNode(const unsigned long int addr, std::set<std::string> itemSet)
{
  _c_address = addr;
  addr_j = addr % AMAX;
  addr_i = (addr - addr_j) / AMAX;
  _q_address = addr_j*AMAX + addr_i;
  _itemSet = itemSet;
}

unsigned long int AddressedNode::getAddress(bool cache)
{
    if (cache) { return _c_address; }
    else { return _q_address; }
}
/**
unsigned long int AddressedNode::getCacheAddress()
{
  return _c_address;
}
unsigned long int AddressedNode::getQueryAddress()
{
  return _q_address;
}
*/
unsigned long int AddressedNode::getDistanceTo(unsigned long int t_addr, bool cache)
{
  unsigned long int this_addr;
  if (cache) { this_addr = _c_address; }
  else { this_addr = _q_address;}
  _small = std::min (this_addr, t_addr);
  _big = std::max (this_addr, t_addr);
  _dist = std::min ((_big-_small), ( WMAX - _big + _small +1));
  return _dist;
}

void AddressedNode::insertItem(std::string item)
{
    _itemSet.insert(item);
}

void AddressedNode::deleteItem(std::string item)
{
    _itemSet.erase(item);
}
