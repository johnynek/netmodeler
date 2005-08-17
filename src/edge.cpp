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

#include "edge.h"
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace Starsky;

Edge::Edge (Node * start, Node * end, double a_weight)
: weight(a_weight)
{
  first = std::min(start, end);
  second = std::max(start, end);
}

Edge::Edge(Node* start, Node* end, const std::string& attr)
{
  first = std::min(start, end);
  second = std::max(start, end);
  weight = atof( attr.c_str() );
}

bool Edge::connects (Node * start, Node * end) const
{
  return ((first == start) && (second == end)) ||
    ((first == end) && (second == start));
}

Node* Edge::getOtherNode (Node * start) const
{
  if (start == first) {
    return second;
  }
  else if (start == second) {
    return first;
  }
  else {
    return 0;
  }
}

std::string Edge::getAttributes() const
{
  if( weight != 1.0 ) {
    std::stringstream attrs;
    attrs << getWeight();
    return attrs.str();
  }
  else {
    return "";
  }
}

bool Edge::hasNode (Node * node) const
{
  return (first == node) || (second == node);
}

double Edge::getWeight() const
{
  return weight;
}
		
void Edge::setWeight(double a_weight)
{
  weight = a_weight;
}

std::string Edge::toString() const {
  //std::cout << "About to call Edge::toString" << std::endl;
  std::stringstream out;
  out << first->toString() << " : " <<
	 second->toString();
  if( weight != 1.0 ) {
    out << " : " << weight;
  }
  return out.str();
}

bool Edge::operator<(const Edge& b) const
{
  if( first == b.first) {
    return (second < b.second);
  }
  else {
    return (first < b.first);
  }
}

bool Edge::operator==(const Edge& a) const
{
  return (first == a.first) && (second == a.second);
}
