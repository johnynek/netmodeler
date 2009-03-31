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

#include "weightededge.h"
#include <iostream>
#include <sstream>
#include <typeinfo>

using namespace Starsky;

WeightedEdge::WeightedEdge (Node * start, Node * end, double w) : Edge(start,end), _weight(w)
{
}

WeightedEdge::WeightedEdge(Node* start, Node* end, const std::string& attr) : Edge(start,end)
{
  std::istringstream( attr ) >> _weight;
}

std::string WeightedEdge::getAttributes() const
{
  if( getWeight() != 1.0 ) {
    std::stringstream attrs;
    attrs << getWeight();
    return attrs.str();
  }
  else {
    return "";
  }
}

double WeightedEdge::getWeight() const
{
  return _weight;
}
		
std::string WeightedEdge::toString() const {
  //std::cout << "About to call WeightedEdge::toString" << std::endl;
  std::stringstream out;
  out << first->toString() << " : " <<
	 second->toString();
  if( _weight != 1.0 ) {
    out << " : " << _weight;
  }
  return out.str();
}

bool WeightedEdge::operator<(const Edge& b) const
{
  bool eq = Edge::operator==(b);
  if( !eq ) {
    //If the base doesn't think they are equal, use the base comparison:
    return Edge::operator<(b);
  }
  else {
    //They appear equal, so compare the weights:
    return getWeight() < b.getWeight();
  }
}

bool WeightedEdge::operator==(const Edge& a) const
{
  try {
    const WeightedEdge& we = dynamic_cast<const WeightedEdge&>(a);
    return (first == a.first) && (second == a.second)
	    && (_weight == a.getWeight());
  }
  catch (std::bad_cast) {
    return false;
  }
}
