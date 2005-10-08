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

#include "walkandpercmessage.h"

using namespace Starsky;
using namespace std;

WalkAndPercMessage::WalkAndPercMessage(Random& r, double p, int walkttl, int percttl) :
   PercolationMessage(r,p,percttl), _ac_mes(r,1,walkttl), _rand(r), _prob(p) {

}
/**
 * this algorithm is based on the breadth first search Starsky::Network::getDistance.
 */
Network* WalkAndPercMessage::visit(Node* n, Network& net) {

    Network* new_net = _ac_mes.visit(n,net);
    auto_ptr<NodeIterator> ni( new_net->getNodeIterator() );
    while( ni->moveNext() ) {
      Node* this_node = ni->current();
      auto_ptr<Network> tmp_net( PercolationMessage::visit(this_node,net) );
      new_net->add( tmp_net.get() );
    }
    return new_net;
}
