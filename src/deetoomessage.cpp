/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida
Copyright (C) 2006  Tae Woong Choi  <twchoi@ufl.edu>, University of Florida

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

DeetooMessage::DeetooMessage(unsigned long int r0, unsigned long int r1, bool cache) : _r0(r0), _r1(r1), _cache(cache)
{
  if (r0 > r1) {
	cerr << "starting point should be less than ending point" << endl
		<< "(" << _r0 << ", " << _r1 << ")" << endl;
  }
  _mid_range = (unsigned long int)( ( (double)(_r0)+(double)(_r1) )/(double)(2) );
  out_edge_count = 1;
  init_node = NULL;
  _dist_to_lower = 4294967295;
}

bool DeetooMessage::inRange(AddressedNode* inode)
{
    unsigned long int nd_addr = inode->getAddress(_cache);
    return ( ( nd_addr >= _r0) && ( nd_addr <= _r1) );
}	

DeetooNetwork* DeetooMessage::visit(Node* n, Network& net)
{
  DeetooNetwork* d2n = dynamic_cast<DeetooNetwork*>( net.newNetwork() );
  AddressedNode* start = dynamic_cast<AddressedNode*> (n);      //start node for broadcasting 
  visit(start, net, *d2n);
  return d2n;
}

void DeetooMessage::visit(AddressedNode* start, Network& net, DeetooNetwork& visited_net)
{
  //cout << "start node: " << start->getAddress(_cache) << endl;
  // If start node is not in the range(_r0, _r1), find the closest neighbor to lower bound in range.
  if ( (!inRange(start) ) )  //node is not in this range
  {
      AddressedNode* next_node=NULL;
      unsigned long int current_dist_to_lower = _dist_to_lower;
      auto_ptr<NodeIterator> ni(net.getNeighborIterator(start) );
      while (ni->moveNext()  )
      {
          AddressedNode* c_node = dynamic_cast<AddressedNode*> (ni->current() );
	  if ( inRange(c_node) ) {
	      init_node = c_node;
	      return visit(c_node, net, visited_net);
	  }
	  else {
	      unsigned long int dist = c_node->getDistanceTo(_mid_range, _cache) ;
              if ( dist < _dist_to_lower )
	      {
                  next_node = c_node;
	          _dist_to_lower = dist;
	      }
	  }
      }	
      if (current_dist_to_lower == _dist_to_lower) {
	  return;
      }
      //We have the closest neighbor to lower, start over there
      out_edge_count++;
      return visit(next_node, net, visited_net);
  }
  if ( init_node == NULL ) { init_node = start; }
  visited_net.add(start);
  //We are in the range, get the neighbors.
  //divide range to upper and lower.
  //get upper neighbors and lower neighbors.
  //std::map will sort them according to their address, lowest first.
  //will divide neighbors to upeer and lower groups wrt its address
  std::map<unsigned long int, AddressedNode*> lower_neighbors;  
  std::map<unsigned long int, AddressedNode*> upper_neighbors;
  auto_ptr<NodeIterator> ni(net.getNeighborIterator(start) );
  while(ni->moveNext() )
  {
    AddressedNode* current_node = dynamic_cast<AddressedNode*> (ni->current() );
    //check if current node is within the range
    if (inRange(current_node) ) {
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
  AddressedNode* last_node_low = NULL;
  std::map<unsigned long int, AddressedNode*>::iterator it_low;
  for (it_low=lower_neighbors.begin(); it_low!=lower_neighbors.end(); it_low++)
  {
        //We don't need to add the node, it is also done when we add an edge
        //visited_net.add(it_low->second);
	visited_net.add(Edge(start, it_low->second) );
	DeetooMessage m_low = DeetooMessage(last_lower, it_low->first, _cache);
	//Here is the recursion.  Note we don't make a new network
	m_low.visit(it_low->second, net, visited_net);
	last_lower = it_low->first +1;
	last_node_low = it_low->second;
  }
  /**
   * I ignore the last bit since its direct neighbor, there's no node between start node and direct node.
   * I will recover this functionality later under churn.
  if (last_node!=NULL) {
    DeetooMessage* m_l = new DeetooMessage(_item, last_node->getAddress(_cache),start->getAddress(_cache)-1,_cache);
    visited_net->add( m_l->visit(last_node, net));
    delete m_l;
  }
  */
    
  //go to the uppper side.
  unsigned long int last_upper = _r1;
  AddressedNode* last_node_up = NULL;
  std::map<unsigned long int, AddressedNode*>::reverse_iterator it_up;
  for (it_up=upper_neighbors.rbegin(); it_up!=upper_neighbors.rend(); it_up++)
  {
        //We don't need to add the node, it is also done when we add an edge
          //visited_net.add(it_up->second);
	  visited_net.add(Edge(start, it_up->second) );
	  DeetooMessage m_up = DeetooMessage(it_up->first, last_upper, _cache);
	  //Here is the recursion.  Note we don't make a new network
	  m_up.visit(it_up->second, net, visited_net);
	  last_upper = it_up->first -1;
	  last_node_up = it_up->second;
  }
  /**
   * I ignore the last bit since its direct neighbor, there's no node between start node and direct node.
   * I will recover this functionality later under churn.
  if (lastNodeUp!=NULL) 
  {
    DeetooMessage* m_u = new DeetooMessage(_item, last_node->getAddress(_cache),_r1, _cache);
    visited_net->add( m_u->visit(last_node, net));
    delete m_u;
  }
  */
    
  return;
}
