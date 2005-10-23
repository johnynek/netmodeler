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

#ifndef starsky__boundingiterator_h
#define starsky__boundingiterator_h

#include <iterator.h>

namespace Starsky {

/**
 * We may want to select only a subset of nodes from an Iterator,
 */
template<typename C, typename Ret, typename Arg>
class BoundingIterator : public Iterator<Arg> {

  public:
    /**
     * @param it the iterator to filter and delete when we are deleted.
     * @param obj the object to call the function on
     * @param methpoint the method to execute on obj
     * @param value the value to compare to
     * @param gt true if we should use greater than as the comparison
     * @param eq true if we should include equal
     */
    BoundingIterator(Iterator<Arg>* it, C* obj,
		     Ret (C::*methpoint) (Arg) const,
		     Ret value, bool gt, bool eq)
    {
      _it = it;
      _obj = obj;
      _methp = methpoint;
      _val = value;
      _gt = gt;
      _eq = eq;
    }

    virtual ~BoundingIterator() {
      delete _it;
    }

    /**
     * Return a copy
     */
    virtual Iterator<Arg>* clone() {
      Iterator<Arg>* new_it = _it->clone();
      return new BoundingIterator<C,Ret,Arg>(new_it,
		                             _obj, _methp, _val, _gt, _eq);
    }

    virtual const Arg& current() {
      return _it->current();
    }

    virtual bool moveNext() {
      bool n = _it->moveNext();
      bool cont = true;
      while( n && cont ) {
	const Arg& argument = _it->current();
        Ret rv = (_obj->*_methp)(argument);
	if( _eq ) {
          //We should accept values which are equal, so stop if equal:
	  if( rv == _val ) {
            cont = false;
	  }
	  else {
            cont = true;
	  }
	}
 	if( cont ) {
	  if( _gt ) {
	    /*
	     * We stop if rv > _val;
	     * We continue if rv is less than _val
	     */
	    if( rv > _val ) {
              cont = false;
	    }
	    else {
              cont = true;
	    }
	  }
	  else {
	    if( rv < _val ) {
              cont = false;
	    }
	    else {
              cont = true;
	    }
	  }
	}
	if( cont ) {
          //We are looping again:
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
    Ret (C::*_methp)(Arg) const;
    Ret _val;
    bool _gt;
    bool _eq;
};

//Here are some useful typedefs:

typedef BoundingIterator<Network, double, Edge*> NDEBIterator;
typedef BoundingIterator<Network, int, Edge*> NIEBIterator;
typedef BoundingIterator<Network, double, Node*> NDNBIterator;
typedef BoundingIterator<Network, int, Node*> NINBIterator;

}
#endif

