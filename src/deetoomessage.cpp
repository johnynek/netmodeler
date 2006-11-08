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
#include <assert.h>

using namespace Starsky;
using namespace std;

#define BMAX 65536
//DeetooMessage::DeetooMessage(int ttl, int nodesz, int hops) : _ttl(ttl), _nodes(nodesz), hops(0)
DeetooMessage::DeetooMessage(std::string item, unsigned long int r0, unsigned long int r1, bool cache) : _item(item), _r0(r0), _r1(r1), _cache(cache)
{
    hit = false;
}

bool DeetooMessage::inRange(AddressedNode* inode)
{
    unsigned long int nd_addr = inode->getAddress(_cache);
    return ( ( nd_addr >= _r0) && ( nd_addr <= _r1) );
}	

DeetooNetwork* DeetooMessage::visit(Node* n, Network& net)
{
  DeetooNetwork* visited_net = dynamic_cast<DeetooNetwork*> (net.newNetwork() );
  AddressedNode* start = dynamic_cast<AddressedNode*> (n);
  AddressedNode* next_node;
  std::map<unsigned long int, AddressedNode*> lower_neighbors;
  std::map<unsigned long int, AddressedNode*> upper_neighbors;
  lower_neighbors.clear();
  upper_neighbors.clear();
  if (_cache)
  {
    if (!inRange(start) )  //node is not in this range
    {
        unsigned long int dist_to_lower = 0;
        auto_ptr<NodeIterator> ni(net.getNeighborIterator(start) );
        while (ni->moveNext() )
	{
	    AddressedNode* c_node = dynamic_cast<AddressedNode*> (ni->current() );
            if ( ( c_node->getDistanceTo(_r0, _cache) < dist_to_lower) || (dist_to_lower < 0) )
	    {
                next_node = c_node;
		dist_to_lower = next_node->getDistanceTo(_r0, _cache);
	    }
	}	
	cout << "next_node's addr: " << next_node->getAddress(_cache) << endl;
	cout << "dist_to_lower: " << dist_to_lower << endl;
	//We have the closest neighbor to lower, start over there
        visited_net->add(visit(next_node, net) );
    }
    else 
    {
        //We are in the range, get the neighbors which are also in range.
	//divide range to upper and lower.
	//get upper neighbors and lower neighbors.
	//std::map will sort them according to their addree, lowest first.
        auto_ptr<NodeIterator> ni(net.getNeighborIterator(start) );
	while(ni->moveNext())
	{
	    AddressedNode* current_node = dynamic_cast<AddressedNode*> (ni->current() );
	    if (inRange(current_node) )
	    {
	      unsigned long int c_node_addr = current_node->getAddress(_cache);
	      cout << "c_node_addr: " << c_node_addr << endl;
	      cout << "source node's addr: " << start->getAddress(_cache) << endl;
	      if (c_node_addr < start->getAddress(_cache) )
	      {
                lower_neighbors[c_node_addr]=current_node; 
	      }
	      else if (c_node_addr > start->getAddress(_cache) )
	      {
                upper_neighbors[c_node_addr]=current_node;
	      }
	    }
        }
        
    }

    //cout << "================================================== " << endl;
    //Start with lower neighbors first.
    cout << " lower_neighbor's size: " << lower_neighbors.size() <<endl;
    unsigned long int last_lower = _r0;
    AddressedNode* last_node;
    std::map<unsigned long int, AddressedNode*>::iterator it_low;
    for (it_low=lower_neighbors.begin(); it_low!=lower_neighbors.end(); it_low++)
    { 
        cout << "last lower:  " << last_lower << endl;
	cout << "this node's addr: " << it_low->first << endl;
	DeetooMessage* m_low = new DeetooMessage(_item, last_lower, it_low->first, _cache);
        if (inRange( it_low->second) )
	{
	    cout << "\t in range" << endl;
            visited_net->add(it_low->second);
	    (it_low->second)->insertItem(_item);
            cout << "================================================== " << endl;
            cout << "\tlast lower:  " << last_lower << endl;
            cout << "\tthis node's addr:  " << it_low->first << endl;
	    //DeetooMessage* m_low = new DeetooMessage(_item, last_lower, it_low->first, _cache);
            cout << "================================================== " << endl;
	    visited_net->add( m_low->visit(it_low->second, net) );
	    last_lower = it_low->first -1;
	    assert(last_lower>=0);
	    last_node = it_low->second;
	    assert(last_node>=0);
	    //delete m_low;
	}
	delete m_low;
		
    }
    cout << "================================================== " << endl;
    cout << "last node's addr: " << last_node->getAddress(_cache) << endl;
    cout << "start node's addr: " << start->getAddress(_cache) << endl;
    DeetooMessage* m_l = new DeetooMessage(_item, last_node->getAddress(_cache),start->getAddress(_cache)-1,_cache);
    cout << "================================================== " << endl;
    visited_net->add( m_l->visit(last_node, net));
    cout << "================================================== " << endl;
    //delete m_l;
    //put us in
    visited_net->add(start);
    
    //go to the uppper side.
    last_lower = start->getAddress(_cache) + 1;
    std::map<unsigned long int, AddressedNode*>::iterator it_up;
    for (it_up=upper_neighbors.begin(); it_up!=upper_neighbors.end(); it_up++)
    {
        if (inRange( (*it_up).second) )
	{
            visited_net->add((*it_up).second);
	    (*it_low).second->insertItem(_item);
	    DeetooMessage* m_up = new DeetooMessage(_item, last_lower, (*it_low).first, _cache);
	    visited_net->add( m_up->visit((*it_low).second, net) );
	    last_lower = (*it_low).first -1;
	    last_node = (*it_low).second;
	    delete m_up;
	}	
    }
    DeetooMessage* m_u = new DeetooMessage(_item, last_node->getAddress(_cache),_r1, _cache);
    visited_net->add( m_u->visit(last_node, net));
    delete m_u;
    lower_neighbors.clear();
    upper_neighbors.clear();
  }
  else
  {
    DeetooNetwork* visited_net = dynamic_cast<DeetooNetwork*> (net.newNetwork() );
    AddressedNode* start = dynamic_cast<AddressedNode*> (n);
    AddressedNode* next_node;
    std::map<unsigned long int, AddressedNode*> lower_neighbors;
    std::map<unsigned long int, AddressedNode*> upper_neighbors;
    lower_neighbors.clear();
    upper_neighbors.clear();
    if (!inRange(start) )  //node is not in this range
    {
        unsigned long int dist_to_lower = 0;
        auto_ptr<NodeIterator> ni(net.getNeighborIterator(start) );
        while (ni->moveNext() )
	{
	    AddressedNode* c_node = dynamic_cast<AddressedNode*> (ni->current() );
            if ( ( c_node->getDistanceTo(_r0, _cache) < dist_to_lower) || (dist_to_lower < 0) )
	    {
                next_node = c_node;
		dist_to_lower = next_node->getDistanceTo(_r0, _cache);
	    }
	}	
	//We have the closest neighbor to lower, start over there
        visited_net->add(visit(next_node, net) );
    }
    else 
    {
        //We are in the range, get the neighbors.
	//divide range to upper and lower.
	//get upper neighbors and lower neighbors.
	//std::map will sort them according to their addree, lowest first.
        auto_ptr<NodeIterator> ni(net.getNeighborIterator(start) );
	while(ni->moveNext() )
	{
	    AddressedNode* current_node = dynamic_cast<AddressedNode*> (ni->current() );
	    unsigned long int c_node_addr = current_node->getAddress(_cache);
	    if (c_node_addr < start->getAddress(_cache) )
	    {
                lower_neighbors[c_node_addr]=current_node; 
	    }
	    else if (c_node_addr > start->getAddress(_cache) )
	    {
                upper_neighbors[c_node_addr]=current_node;
	    }
        }
        
    }

    //Start with lower neighbors first.
    unsigned long int last_lower = _r0;
    AddressedNode* last_node;
    std::map<unsigned long int, AddressedNode*>::iterator it_low;
    for (it_low=lower_neighbors.begin(); it_low!=lower_neighbors.end(); it_low++)
    {
        if (inRange( (*it_low).second) )
	{
            visited_net->add((*it_low).second);
	    std::set<std::string> this_itemSet = (*it_low).second->getItem();
	    if (this_itemSet.find(_item) != this_itemSet.end() ) 
	    {
		hit = true;
		goto stop;
	    }
	    DeetooMessage* m_low = new DeetooMessage(_item, last_lower, (*it_low).first, _cache);
	    visited_net->add( m_low->visit((*it_low).second, net) );
	    last_lower = (*it_low).first -1;
	    last_node = (*it_low).second;
	    delete m_low;
	}	
    }
    DeetooMessage* m_l = new DeetooMessage(_item, last_node->getAddress(_cache),start->getAddress(_cache)-1,_cache);
    visited_net->add( m_l->visit(last_node, net));
    delete m_l;
    //put us in
    visited_net->add(start);
    
    //go to the uppper side.
    last_lower = start->getAddress(_cache) + 1;
    std::map<unsigned long int, AddressedNode*>::iterator it_up;
    for (it_up=upper_neighbors.begin(); it_up!=upper_neighbors.end(); it_up++)
    {
        if (inRange( (*it_up).second) )
	{
            visited_net->add((*it_up).second);
	    std::set<std::string> this_itemSet = (*it_low).second->getItem();
	    if (this_itemSet.find(_item) != this_itemSet.end() ) 
	    {
		hit = true;
		goto stop;
	    }
	    DeetooMessage* m_up = new DeetooMessage(_item, last_lower, (*it_low).first, _cache);
	    visited_net->add( m_up->visit((*it_low).second, net) );
	    last_lower = (*it_low).first -1;
	    last_node = (*it_low).second;
	    delete m_up;
	}	
    }
    DeetooMessage* m_u = new DeetooMessage(_item, last_node->getAddress(_cache),_r1, _cache);
    visited_net->add( m_u->visit(last_node, net));
    delete m_u;
    lower_neighbors.clear();
    upper_neighbors.clear();
  }
  stop:
  return visited_net;
}
