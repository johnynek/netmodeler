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

#ifndef starsky__node_h
#define starsky__node_h

#include <string>
#include <sstream>

namespace Starsky {

  /**
   * Base node class.  Subclass this to make different types of nodes.
   * We usually work with pointers to nodes, so we don't have to store O(N^2)
   * copies of the data associated with each node.
   * @see Starsky::Network
   * @see Starsky::Edge
   */
  class Node {
  public:
      Node();
      Node(const std::string& s) { _name = s; }
      virtual ~Node() { };
      /**
       * @return a string suitable for printing
       */
      virtual std::string toString() const {
        if( _name != "" ) {
          return _name;
	}
	std::stringstream s;
	s << "n" << this;
        return s.str();
      }
  protected:
      static int _node_count;
      std::string _name;
  };

}
#endif
