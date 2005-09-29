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

#ifndef starsky__edgeenumerator_h
#define starsky__edgeenumerator_h

#include <node.h>
#include <edge.h>
#include <map>
#include <set>
#include <exception>

namespace Starsky {

//Let the EdgeIterator know that Networks exist:
class Network;
	
/**
 * This class abstracts any container system we are using to
 * loop over all nodes in a network.
 *
 * In order to make it easier to wrap netmodeler with another language,
 * we prefer not to directly expose STL elements in the API.
 */
class EdgeIterator {

  public:
    /**
     * @throw an exception if this is called before MoveNext is called once.
     * @return the Edge that the iterator currently points to
     * */
    Edge* current();
    /**
     * Go to the next Edge.  Return true if there is another Edge
     * This must be called before you first call Current.  So,
     * you can think of the Iterator as positioned BEFORE the
     * start, and so you must move it once to be in position.
     * This is so the while( it.MoveNext() ) { } idiom will
     * work.
     */
    bool moveNext();
    /**
     * Go back to the start of the network.
     * Must call MoveNext before we can call current again
     */
    void reset();
    //Allow Network to modify protected and private members
    friend class Network;
  protected:
    std::map< Node*, std::set<Edge*> >::const_iterator _nit;
    std::map< Node*, std::set<Edge*> >::const_iterator _begin;
    std::map< Node*, std::set<Edge*> >::const_iterator _end;
    std::set<Edge*>::const_iterator _eit;
    bool _called_movenext;
};
	
}

#endif
