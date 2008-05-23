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
#include "cnt_ptr.h"

namespace Starsky {

/**
 * This is an Iterator for any STL Container model.
 * @see http://www.sgi.com/tech/stl/Container.html
 */
template<class Cont>
class ContainerIterator : public Iterator< typename Cont::value_type > {
  public:
    ContainerIterator(const Cont& con) : _con(con) {
      reset();
    }

    ContainerIterator<Cont>* clone() {
      return new ContainerIterator<Cont>(_con);
    }
    
    const typename Cont::value_type& current() {
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
 * This is an Iterator for any STL Container model.
 * @see http://www.sgi.com/tech/stl/Container.html
 * This deletes the pointer to the original container when all clones
 * are deleted.
 */
template<class Cont>
class ContainerPIterator : public Iterator< typename Cont::value_type > {
  public:
    ContainerPIterator(Cont* con) : _con(con) {
      reset();
    }

    ContainerPIterator<Cont>* clone() {
      return new ContainerPIterator<Cont>(_con);
    }
    
    const typename Cont::value_type& current() {
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
    ContainerPIterator&  operator = (const ContainerPIterator& other) { /*..*/}
    //Don't allow copy constructors.
    ContainerPIterator(const ContainerPIterator& other) { /*..*/ }
    ContainerPIterator(cnt_ptr<Cont>& con) : _con(con) {
      reset();
    }
    ContainerPIterator() { }

  protected:
    typename Cont::const_iterator _it;
    cnt_ptr<Cont> _con;
    bool _moved_to_first;
};

}

#endif
