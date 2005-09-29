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

#include "prefdelcompnetwork.h"

using namespace Starsky;
using namespace std;

PrefDelCompNetwork::PrefDelCompNetwork(const Network& seed,
		                       Random& r,
				       double p,
				       int connections,
				       double q) : PrefAtNetwork(seed,
					                         r,
							         connections),
				                                 _del_p(p),
								 _restore_q(q)
{

}

PrefDelCompNetwork::PrefDelCompNetwork(const Network& seed,
		                       Random& r,
				       const DegreePreferenceFunction& dp,
				       double p,
				       int connections,
				       double q) : PrefAtNetwork(seed,
					                         r,
								 dp,
								 connections),
				                                 _del_p(p),
								 _restore_q(q)
{

}	

const Network::NodePSet PrefDelCompNetwork::getRemoveNodes()
{
  if( _rand.getBool( _del_p ) && (getNodeSize() > 1)) {
    NodePSet::const_iterator nit;

    int node = _rand.getInt(getNodeSize() - 1);
    NodeIterator ni = getNodeIterator();
    ni.moveNext();
    while( node-- > 0) { ni.moveNext(); }
    NodePSet removenodes;
    removenodes.insert(ni.current());
    return removenodes;
  }
  return _empty_nodeset;
}

bool PrefDelCompNetwork::needsCompensation(Node* n, Edge* e)
{
  return _rand.getBool( _restore_q );
}
