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

#include "walkandsitepercmessage.h"

using namespace Starsky;
using namespace std;

WalkAndSitePercMessage::WalkAndSitePercMessage(Random& r, double p, int walkttl, int percttl) :
   SitePercMessage(r,p,percttl), _ac_mes(r,1,walkttl), _rand(r), _prob(p) {

}
/**
 * this algorithm is based on the breadth first search Starsky::Network::getDistance.
 */
void WalkAndSitePercMessage::visit(Node* n, Network& net) {

    _ac_mes.forgetVisitedNodes();
    _ac_mes.visit(n,net);
    _crossed_edges += _ac_mes.getCrossedEdgeCount();
    set<Node*>::const_iterator v_it;
    for(v_it = _ac_mes.getVisitedNodes().begin(); v_it != _ac_mes.getVisitedNodes().end(); v_it++) {
        SitePercMessage::visit(*v_it,net);
    }
}

