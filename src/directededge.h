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
        DirectedEdge(Node* start=0, Node* end=0);
        virtual bool connects(Node* start, Node* end) const;
	Node* getStartNode() const;
	Node* getEndNode() const;
	bool pointsFirstToSecond() const { return _dir_first_to_second; }
	void reverse();

	std::string toString() const;
    
        bool operator<(const Edge& a) const; 
        bool operator==(const Edge& a) const;
    
    protected:
        bool _dir_first_to_second; // is true of the direction is from first to second
};
	
}

#endif
