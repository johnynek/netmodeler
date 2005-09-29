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

#include "trianglemapper.h"

#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

using namespace Starsky;

void TriangleMapper::map(Network* net)
{
  Network::EdgeSet to_remove;
  EdgeIterator ei = net->getEdgeIterator();
  while( ei.moveNext() ) {
    Edge* this_edge = ei.current();
    if( net->getDegree( this_edge->first ) == 1 ||
        net->getDegree( this_edge->second ) == 1 ) {
      //This is a dead-end, keep it.
    }
    else {
      int tri = net->getTriangles( this_edge );
      if( tri == 0 ) {
        //This edge is not in any triangles
        to_remove.insert( this_edge );
      }
    }
  }
  //Actually remove the edges:
  Network::EdgeSet::iterator eit;
  FOREACH( eit, to_remove ) {
    net->remove( *eit );
  }
}
