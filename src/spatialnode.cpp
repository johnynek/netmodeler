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

#include "spatialnode.h"

using namespace Starsky;
using namespace std;

SpatialNode::SpatialNode(Random& rand, int dim) {
    _coords.resize(dim);
    
    while(dim-- > 0) {
        _coords.push_back( rand.getDouble01() );
    }
}

SpatialNode::SpatialNode(const vector<double>& pos) : _coords(pos) { }

const vector<double>& SpatialNode::getPosition() const {
  return _coords;
}

double SpatialNode::getDistanceTo(SpatialNode* a_node) const {
	
    const vector<double>& other_pos = a_node->getPosition();
    if( other_pos.size() != _coords.size() ) {
        return -1.0;
    }

    unsigned int i;
    double dist2 = 0.0;
    for( i = 0; i < _coords.size(); i++) {
        dist2 += ( other_pos[i] - _coords[i] ) * ( other_pos[i] - _coords[i] );
    }
    return sqrt( dist2 );
}
