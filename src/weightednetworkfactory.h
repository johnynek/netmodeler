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

#ifndef starsky__weightednetworkfactory_h
#define starsky__weightednetworkfactory_h

#include <networkfactory.h>
#include <weightednetwork.h>
#include <weightededgefactory.h>

namespace Starsky {

/**
 * Base class for making Networks.
 * This can read from different file types,
 * or make different networks according to
 * some algorithm
 */
class WeightedNetworkFactory : public NetworkFactory {

  public:
    WeightedNetworkFactory();
    /**
     * These pointers are deleted when this NetworkFactory
     * is deleted
     */
    WeightedNetworkFactory(NodeFactory* nf, EdgeFactory* ef);
   
    virtual Network* create() { return 0; }
    /**
     * Read from a file
     */
    virtual Network* create(std::istream& input);
};

}

#endif
