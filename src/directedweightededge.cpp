/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
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

#include "directedweightededge.h"

using namespace Starsky;

DirectedWeightedEdge::DirectedWeightedEdge(Node* start, Node* end, double weight) :
  DirectedEdge(start,end)
{
  _weight = weight;
}

bool DirectedWeightedEdge::operator<(const Edge& e) const
{
    try {
      //See if this is a directed edge
      const DirectedWeightedEdge& d_a =
              dynamic_cast<const DirectedWeightedEdge&>(e);

      bool first_eq = (first == d_a.first);

      if( first_eq ) {
        bool second_eq = (second == d_a.second);
        if( second_eq ) {
           //The first and second are equal, do they point the same way?
           bool same_pt = _dir_first_to_second == d_a.pointsFirstToSecond();
           if( same_pt ) {
             return (getWeight() < e.getWeight());
           }
           else {
             //define false < true
             return !_dir_first_to_second;
           }
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

bool DirectedWeightedEdge::operator==(const Edge& e) const
{
  return DirectedEdge::operator==(e) && (getWeight() == e.getWeight());
}
