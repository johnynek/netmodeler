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

#include <deetoomessage.h>

using namespace Starsky;
using namespace std;

#define BMAX 65536
//DeetooMessage::DeetooMessage(int ttl, int nodesz, int hops) : _ttl(ttl), _nodes(nodesz), hops(0)
DeetooMessage::DeetooMessage(std::string query, unsigned long int r0, unsigned long int r1) : _query(query), _r0(r0), _r1(r1)
{
}

DeetooNetwork* DeetooMessage::visit(AddressedNode* n, DeetooNetwork& net)
{
    /* 
     * Determine cache or query size
     */
    int cqsize = int( sqrt( BMAX / net.getNodeSize() ) );
    // local broadcasting range [addr_i*BMAX, addr_i*BMAX*cqsize+BAMX-1]
    //                       or [(addr_i-2/cqsize)*BMAX, (addr_i+2/cqsize)*BMAX+BMAX-1 ]
    unsigned long int start_addr = (unsigned long int)(n->addr_i-2/cqsize)*BMAX;
    unsigned long int end_addr = (unsigned long int)( ( (start_addr/BMAX)+cqsize)*BMAX + BMAX -1);
    std::map<unsigned long int, AddressedNode*> new_nm;
    std::map<unsigned long int, AddressedNode*>::iterator itNodeMap ;
    for (itNodeMap=net.node_map.begin(); itNodeMap!=net.node_map.end(); itNodeMap++) {
	if (itNodeMap->first >= start_addr && itNodeMap->first <= end_addr) {
            new_nm.insert(*itNodeMap);
	}

    }
    
    
       
    map<int, Network::NodePSet > to_visit;
    map<int, Network::NodePSet >::iterator tv_it;
    Network::NodePSet::iterator a_it;

    to_visit[0].insert(n);
    DeetooNetwork* new_net = net.newNetwork();
    new_net->add( n );
    int this_distance;
    //We loop through at each TTL:
    tv_it = to_visit.begin();
    //while( tv_it != to_visit.end() && ( (tv_it->first < _ttl) || (_ttl == -1) ) ) {
    while( tv_it != to_visit.end() ) {
        this_distance = tv_it->first + 1;
        //Here are all the nodes at this distance:
        for( a_it = tv_it->second.begin(); a_it != tv_it->second.end(); a_it++) {
            auto_ptr<EdgeIterator> ei( net.getEdgeIterator(*a_it) );
            while( ei->moveNext() ) {
	      Edge* this_edge = ei->current();
              AddressedNode* this_node = dynamic_cast<AddressedNode*> (this_edge->getOtherNode(*a_it) );
	      //Make sure to consider potential directionality 
              if( this_edge->connects(*a_it, this_node) ) {
		 //if( !new_net->has( this_node ) ) {
		 if( !new_net->has( this_node ) && new_nm.find(this_node->getQueryAddress() )!= new_nm.end() )   {
                  //We have not seen this one yet.
                  to_visit[this_distance].insert( this_node );
		  new_net->add( this_node );
		 }
		 //We have to cross it, only the other node knows if it should pass it
		 new_net->add( this_edge );
              }
            }
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
    return new_net;
}
