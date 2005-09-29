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

#include "edgeiterator.h"
#include <iostream>
//#define DEBUG(msg) std::cerr << __FILE__ << " (" << __LINE__ << ") " << msg << std::endl;
#define DEBUG(msg)


using namespace Starsky;
using namespace std;

Edge* EdgeIterator::current()
{
  
  if( !_called_movenext ) {
    throw std::exception();
  }
  if( _eit == _nit->second.end() ) {
    throw exception();
  }
  DEBUG("End of Current");
  return (*_eit);
}

bool EdgeIterator::moveNext()
{
  DEBUG("Start of MoveNext");
  if( false == _called_movenext ) {
    DEBUG("not called movenext");
    _called_movenext = true;
  }
  else {
    DEBUG("Already called movenext");
    _eit++;
  }
  //Advance to the next real edge
  DEBUG("Looking for next edge");
  bool found_next = false;
  while( ( false == found_next ) && ( _nit != _end ) ) {
    /*
     * check to see if we need to advance nit
     */
    if( _eit == _nit->second.end() ) {
      /*
       * We are at the end of one EdgeSet, go to the next:
       */
      if( _nit != _end ) {
        _nit++;
	if( _nit != _end ) {
          /*
           * There are more EdgeSets to look at
           */
          _eit = _nit->second.begin();
          //We think we have found next, we double check in the next if block
          found_next = ( _eit != _nit->second.end() );
        }
      }
      else {
        //There are no more nodes.
        found_next = false;
      }
    }
    else {
      //Looks like we have a candidate edge,
      found_next = true;
    }
    /*
     * Here we check that we don't hit the same edge twice
     * be making sure that edge->first == node
     */
    if( found_next && ( (*_eit)->first != _nit->first ) ) {
      /*
       * Skip edges unless _nit is the first Node.  This is so we don't
       * count edges twice
       */
      _eit++;
      //We have to loop through again to check this new edge
      found_next = false;
    }
    else {
      //found_next AND edge->first == node.  Great!
      //We don't need to say this, but it is true: found_next = true;
    }
  }
  DEBUG("End of EdgeIterator::MoveNext"); 
  return found_next;
}

void EdgeIterator::reset()
{
  _nit = _begin;
  _eit = _nit->second.begin();
  _called_movenext = false;
}
