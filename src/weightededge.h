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

#ifndef starsky_weightededge_h
#define starsky_weightededge_h

#include "edge.h"

namespace Starsky {

class WeightedEdge : public Edge {

  public:
    /**
     * Create a new weighted edge with a given weight
     * @param start the first node
     * @param end the second node
     * @param weight the weight of the edge
     */
    WeightedEdge(Node* start, Node* end, double weight = 1.0);
    /**
     * Create a new edge
     * @param start the first node
     * @param end the second node
     * @param attrs the weight of the edges as string
     */
    WeightedEdge(Node* start, Node* end, const std::string& attrs);
    
    virtual WeightedEdge* clone() const { return new WeightedEdge(first, second, _weight); }
    virtual std::string getAttributes() const;
    virtual double getWeight() const;
    /**
     * @return a string representation of the edge
     */
    virtual std::string toString() const;

    virtual bool operator<(const Edge& a) const;
    virtual bool operator==(const Edge& a) const;
  protected:
    double _weight;
};
	
}

#endif
