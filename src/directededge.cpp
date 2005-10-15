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

#include "directededge.h"

using namespace Starsky;
using namespace std;

DirectedEdge::DirectedEdge(Node* start, Node* end) : Edge(start,end) {
    if( start < end) {
	_dir_first_to_second = true;
    }
    else {
	_dir_first_to_second = false;
    }
}

bool DirectedEdge::connects(Node* start, Node* end) const {
  if( _dir_first_to_second ) {
    return (start == first) && (end == second);
  }
  else {
    return (start == second) && (end == first);
  }
}

Node* DirectedEdge::getStartNode() const{
    if( _dir_first_to_second == true) {
      return first;
    }
    else {
      return second;
    }
}

Node* DirectedEdge::getEndNode() const{
    if( _dir_first_to_second == true) {
      return second;
    }
    else {
      return first;
    }
}

DirectedEdge* DirectedEdge::reverse() const {
    //Switch the direction:
    if( _dir_first_to_second == true) {
      /*
       * this edge is first->second,
       * so return a new one that is second->first
       */
      return new DirectedEdge(second, first);
    }
    else {
      /*
       * this edge is second->first,
       * so return a new one that is first->second
       */
      return new DirectedEdge(first, second);
    }
}

std::string DirectedEdge::toString() const {
  if( _dir_first_to_second ) {
     return "from " + first->toString() + " to " + second->toString();
  }
  else {
     return "from " + second->toString() + " to " + first->toString();
  }  
}

bool DirectedEdge::operator<(const Edge& a) const {

    try {
      //See if this is a directed edge
      const DirectedEdge& d_a = dynamic_cast<const DirectedEdge&>(a);
    
      bool first_eq = (first == d_a.first);

      if( first_eq ) {
	bool second_eq = (second == d_a.second);
        if( second_eq ) {
           return (!_dir_first_to_second) && d_a.pointsFirstToSecond();
	}
	else {
           return (second < d_a.second);
	}
      }
      else {
        return (first < d_a.first);
      }
    }
    catch(std::bad_cast x) {
      //If it is not a subclass of DirectedEdge, it is less than directed edge.
      return true; 
    }
}

bool DirectedEdge::operator==(const Edge& a) const {
  try {
    const DirectedEdge& d_a = dynamic_cast<const DirectedEdge&>(a);
    if ( _dir_first_to_second ) {
      return d_a.connects(first, second);
    }
    else {
      return d_a.connects(second, first);
    }
  }
  catch(std::bad_cast x) {
    //If it is not a subclass of DirectedEdge, it is not equal
    return false;
  }
}
