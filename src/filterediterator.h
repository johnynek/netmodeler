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
#include <node.h>
#include <edge.h>
#include <random.h>

namespace Starsky {

/**
 * We may want to select only a subset of nodes from an Iterator,
 */
template<typename Arg>
class Filterator : public Iterator<Arg> {

  public:
    /**
     * @param it the iterator to filter and delete when we are deleted.
     * @param obj the object to call the function on
     * @param methpoint the method to execute on obj, we skip this item if false
     */
    Filterator(Iterator<Arg>* it)
    {
      _it = it;
    }

    virtual ~Filterator() {
      delete _it;
    }

    /**
     * Return a copy
     */
    virtual Iterator<Arg>* clone() = 0;

    virtual const Arg& current() {
      return _it->current();
    }

    virtual bool moveNext() {
      bool n = _it->moveNext();
      bool cont = true;
      while( n && cont ) {
	//Continue if we should not include the current position
	cont = (include() == false);
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
    /**
     * @return true if current
     * should be included in the iteration, false if v should be skipped.
     */
    virtual bool include() = 0;
    Iterator<Arg>* _it;
};

/**
 * RandomFilterator randomly accepts from a given iterator
 */
template<typename T>
class RandomFilterator : public Filterator<T> {

  public:
    /**
     * Create a new RandomFilterator from the given iterator.  When
     * this object is deleted, so will it.
     */
    RandomFilterator(Iterator<T>* it, Random& r, double p) : Filterator<T>(it), _rand(r), _prob(p) {
      
    }
    virtual Iterator<T>* clone() {
      return new RandomFilterator<T>(Filterator<T>::_it->clone(), _rand, _prob);
    }

  protected:
    bool include() {
      return _rand.getBool(_prob);
    }
    
    Random& _rand;
    double _prob;
	
};

typedef RandomFilterator<Node*> RandomNodeFilterator;
typedef RandomFilterator<Edge*> RandomEdgeFilterator;

/**
 * We may want to select only a subset of nodes from an Iterator,
 */
template<typename C, typename Arg>
class ClassFilterator : public Filterator<Arg> {

  public:
    /**
     * @param it the iterator to filter and delete when we are deleted.
     * @param obj the object to call the function on
     * @param methpoint the method to execute on obj, we skip this item if false
     */
    ClassFilterator(Iterator<Arg>* it, C* obj, bool (C::*methpoint) (Arg)) : Filterator<Arg>(it)
    {
      _obj = obj;
      _methp = methpoint;
    }

    /**
     * Return a copy
     */
    virtual Iterator<Arg>* clone() {
      Iterator<Arg>* new_it = Filterator<Arg>::_it->clone();
      return new ClassFilterator<C,Arg>(new_it, _obj, _methp);
    }

  protected:
    bool include() {
      const Arg& argument = Filterator<Arg>::_it->current();
      return (_obj->*_methp)(argument);
    }
    C* _obj;
    bool (C::*_methp)(Arg);
};
}
#endif

