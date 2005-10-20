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

#ifndef starsky__directededge_h
#define starsky__directededge_h

#include "edge.h"

//The below is to use the bad_cast exception, used in < and ==
#include <typeinfo>

namespace Starsky {

class DirectedEdge : public Edge {

    public: 
	/**
	 * Make a new directed edge that goes from start to end
	 */
        DirectedEdge(Node* start, Node* end);
	virtual DirectedEdge* clone() const {
          if( _dir_first_to_second ) {
	    return new DirectedEdge(first, second);
	  }
	  else {
            return new DirectedEdge(second, first);
	  }
	}
	/**
	 * @param start a node to check to see if it is the start
	 * @param end the node to check to see if it is the end
	 * @return true if this edge goes from start -> end
	 */
        virtual bool connects(Node* start, Node* end) const;
	/**
	 * @return the starting node of the directed edge
	 */
	Node* getStartNode() const;
	/**
	 * @return the ending node of the directed edge
	 */
	Node* getEndNode() const;
	/**
	 * @return true if this node goes from first -> second
	 */
	bool pointsFirstToSecond() const { return _dir_first_to_second; }
	/**
	 * @return a new edge which is the same as this one, but reversed
	 */
	virtual DirectedEdge* reverse() const;

	virtual std::string toString() const;
    
        virtual bool operator<(const Edge& a) const; 
        virtual bool operator==(const Edge& a) const;
    
    protected:
        bool _dir_first_to_second; // is true of the direction is from first to second
};
	
}

#endif
