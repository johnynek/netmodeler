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

#ifndef starsky__edge_h
#define starsky__edge_h

#include "node.h"

namespace Starsky {

  /**
   * Simple object to store pointers to two nodes
   * @see Starsky::Node
   * @see Starsky::Network
   * @see Starsky::DirectedEdge
   */
  class Edge {
  public:
    ///the Node* with the lowest memory address 
    Starsky::Node * first;
    ///the Node* with the lowest memory address
    Starsky::Node * second;

    /**
     * creates an edge between start and end.  For undirected nodes, order does
     * not matter.  @see Starsky::DirectedNode
     */
    Edge (Node * start = 0, Node * end = 0, double a_weight=1.0);
    /**
     * Make an edge with the given attributes.  This
     * edge only supports a weight (double in the string)
     */
    Edge(Node* start, Node* end, const std::string& attrs);

    virtual ~Edge() { };
    /**
     * @param start
     * @param end
     * @return true if this edge has both start and end
     */
    virtual bool connects (Node * start, Node * end) const;
    /**
     * @return true if this node is in this edge
     */
    virtual bool contains(Node* n) const {
      return ((first == n) || (second == n));
    };
    /**
     * @return first if passed second, second if passed first, and 0 otherwise
     */
    Node * getOtherNode (Node * start) const;
    /**
     * Returns the attributes of this edge.  Useful in EdgeFactories
     */
    virtual std::string getAttributes() const;
    /**
     * @return true of this Node is first or second
     */
    bool hasNode (Node *) const;
    virtual std::string toString() const;

    double getWeight() const;
    void setWeight(double a_weight);
			
    virtual bool operator<(const Edge& a) const;
    virtual bool operator==(const Edge& a) const;
	protected:
		///the weight of the edge
		double weight;
	
  };

}
#endif
