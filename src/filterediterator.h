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

#ifndef starsky__filterediterator_h
#define starsky__filterediterator_h

#include <iterator.h>

namespace Starsky {

/**
 * We may want to select only a subset of nodes from an Iterator,
 */
template<typename C, typename Arg>
class FilteredIterator : public Iterator<Arg> {

  public:
    /**
     * @param it the iterator to filter and delete when we are deleted.
     * @param obj the object to call the function on
     * @param methpoint the method to execute on obj, we skip this item if false
     */
    FilteredIterator(Iterator<Arg>* it, C* obj, bool (C::*methpoint) (Arg*))
    {
      _it = it;
      _obj = obj;
      _methp = methpoint;
    }

    virtual ~FilteredIterator() {
      delete _it;
    }

    /**
     * Return a copy
     */
    virtual Iterator<Arg>* clone() {
      Iterator<Arg>* new_it = _it->clone();
      return new FilteredIterator<C,Arg>(new_it, _obj, _methp);
    }

    virtual Arg* current() {
      return _it->current();
    }

    virtual bool moveNext() {
      bool n = _it->moveNext();
      bool cont = true;
      while( n && cont ) {
	Arg* argument = _it->current();
	//Keep going if we should not return this one
	cont = ((_obj->*_methp)(argument) == false);
	if( cont ) {
          n = _it->moveNext();
	}
      }
      return n && (!cont);
    }

    virtual void reset() {
      _it->reset();
    }
    
  protected:
    Iterator<Arg>* _it;
    C* _obj;
    bool (C::*_methp)(Arg*);
};
}
#endif

