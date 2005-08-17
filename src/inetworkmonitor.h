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

#ifndef starsky__inetworkmonitor_h
#define starsky__inetworkmonitor_h

namespace Starsky {

/* To break any circular dependencies we declare
 * the following:
 */
class Node;
class Edge;
class Network;
	
/*
 Monitor when things happen to networks.
 All changes will be announced
 to the INetworkMonitors which are listening.
 The Network will have a vector<INetworkMonitor*>
 which it will iterate through when there is a change
 */

class INetworkMonitor {

  public:
    /**
     * Called after the monitor is added to the network
     */
    virtual void postAdd(Network* net) { };
    virtual void preNodeAdd(Node* newnode) { };
    virtual void postNodeAdd(Node* newnode) { };
    virtual void preNodeRemove(Node* node) { };
    virtual void postNodeRemove(Node* node) { };
    virtual void preEdgeAdd(Edge* newedge) { };
    virtual void postEdgeAdd(Edge* newedge) { };
    virtual void preEdgeRemove(Edge* edge) { };
    virtual void postEdgeRemove(Edge* edge) { };
};

}

#endif
