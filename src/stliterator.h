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

#ifndef starsky_stliterator_h
#define starsky_stliterator_h

#include "iterator.h"

namespace Starsky {

/**
 * This is an Iterator that wraps standard STL iterators.  It would
 * be useful for making Starsky::Iterators out of iterators from any
 * single (non-map) container.
 */
template<typename Cont, typename T>
class StlIterator : public Iterator<T> {

  public:
    StlIterator(const Cont& con) : _con(con) {
      reset();
    }

    StlIterator<Cont, T>* clone() {
      return new StlIterator<Cont,T>(_con);
    }
    
    const T& current() {
      return (*_it);
    }
    
    bool moveNext() {
      if( _it == _con.end() ) {
        return false;
      }
      if( _moved_to_first ) {
        _it++;
      }
      else {
        _moved_to_first = true;
      }
      return ( _it != _con.end() );
    }
    
    void reset() {
      _moved_to_first = false;
      _it = _con.begin();
    }

  protected:
    typename Cont::const_iterator _it;
    const Cont& _con;
    bool _moved_to_first;
};

/**
 * This is an Iterator that wraps standard STL containers.
 * This iterator totally "owns" the container, and deletes the
 * pointer when the last clone is deleted.
 */
template<typename Cont, typename T>
class StlPIterator : public Iterator<T> {

  public:
    StlPIterator(Cont* con) : _con(con) {
      //Initialize the reference count.
      _ref_count = new int[1];
      (*_ref_count) = 1;
      reset();
    }

    StlPIterator<Cont, T>* clone() {
      StlPIterator<Cont, T>* ret_val = new StlPIterator<Cont,T>();
      ret_val->_con = _con;
      ret_val->_ref_count = _ref_count;
      //Increment reference count:
      (*_ref_count)++;
      //Make sure these two are at the same point:
      ret_val->_moved_to_first = _moved_to_first;
      ret_val->_it = _it;
    }

    ~StlPIterator() {
      (*_ref_count)--; 
      if( *_ref_count == 0 ) {
        //No more references:
	delete _con;
	delete _ref_count;
      }
    }
    
    const T& current() {
      return (*_it);
    }
    
    bool moveNext() {
      if( _it == _con->end() ) {
        return false;
      }
      if( _moved_to_first ) {
        _it++;
      }
      else {
        _moved_to_first = true;
      }
      return ( _it != _con->end() );
    }
    
    void reset() {
      _moved_to_first = false;
      _it = _con->begin();
    }

  private:
    //Don't allow copies
    StlPIterator&  operator = (const StlPIterator& other) { /*..*/}
    //Don't allow copy constructors.
    StlPIterator(const StlPIterator& other) {/*..*/}
    StlPIterator() { }
  protected:
    typename Cont::const_iterator _it;
    Cont* _con;
    //Keeps track of the number of references to this Iterator
    int* _ref_count;
    bool _moved_to_first;
};
	
}

#endif
