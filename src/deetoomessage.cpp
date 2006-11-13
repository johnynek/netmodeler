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

//DeetooMessage::DeetooMessage(int ttl, int nodesz, int hops) : _ttl(ttl), _nodes(nodesz), hops(0)
DeetooMessage::DeetooMessage(std::string item, unsigned long int r0, unsigned long int r1, bool cache) : _item(item), _r0(r0), _r1(r1), _cache(cache)
{
    hit = false;
}

bool DeetooMessage::inRange(AddressedNode* inode)
{
    unsigned long int nd_addr = inode->getAddress(_cache);
    //cout << "range_low: " << _r0 << "range_up: " << _r1 << endl
    //	    << "this_node addr: " << nd_addr << endl
    //        << "in range?\t " ;
    //if ( ( nd_addr >= _r0) && ( nd_addr <= _r1) ) { cout << "yes" <<endl;}

    return ( ( nd_addr >= _r0) && ( nd_addr <= _r1) );
}	

DeetooNetwork* DeetooMessage::visit(Node* n, Network& net)
{
  //_cache = false;
  DeetooNetwork* visited_net = dynamic_cast<DeetooNetwork*> (net.newNetwork() );
  //auto_ptr<DeetooNetwork> visited_net ( dynamic_cast<DeetooNetwork*> (net.newNetwork() ) );
  AddressedNode* start = dynamic_cast<AddressedNode*> (n);
  AddressedNode* next_node=NULL;
  std::map<unsigned long int, AddressedNode*> lower_neighbors;
  std::map<unsigned long int, AddressedNode*> upper_neighbors;
  lower_neighbors.clear();
  upper_neighbors.clear();
  //cout << " lower range and upper range:\t" << _r0 << "\t" << _r1 << endl;
  //cout << " current node addr:\t" << start->getAddress(_cache) << endl;
  if (!inRange(start) )  //node is not in this range
  {
      unsigned long int dist_to_lower = 0;
      //cout << "sidtslaejr" << endl;
      auto_ptr<NodeIterator> ni(net.getNeighborIterator(start) );
      //cout << "auto_ptr" << endl;
      while (ni->moveNext() )
      {
          AddressedNode* c_node = dynamic_cast<AddressedNode*> (ni->current() );
          if ( ( c_node->getDistanceTo(_r0, _cache) < dist_to_lower) || (dist_to_lower <= 0) )
	  {
              next_node = c_node;
	      dist_to_lower = next_node->getDistanceTo(_r0, _cache);
	      //cout << "next_node   " << c_node->getAddress(_cache) << endl;
	      //cout << "dist to lower " << dist_to_lower << endl;
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
      //cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;	
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
  AddressedNode* last_node=NULL;
  std::vector<pair<unsigned long int, AddressedNode*> > range_low;
  range_low.clear();
  //cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << endl;
  std::map<unsigned long int, AddressedNode*>::iterator it_low;
  for (it_low=lower_neighbors.begin(); it_low!=lower_neighbors.end(); it_low++)
  {
    if (inRange( it_low->second) )
    {
        visited_net->add(it_low->second);
	std::set<std::string> this_itemSet = (it_low->second)->getItem();
	if (this_itemSet.find(_item) != this_itemSet.end() ) 
	{
	    hit = true;
	    break;
	}
	//DeetooMessage* m_low = new DeetooMessage(_item, last_lower, it_low->first, _cache);
	//visited_net->add( m_low->visit(it_low->second, net) );
	range_low.push_back(make_pair ( last_lower, it_low->second) );
	last_lower = it_low->first +1;
	last_node = it_low->second;
	//delete m_low;
    }	
  }
  visited_net->add(start);
  if (!hit)
  {
    /**
     * I ignore the last bit since its direct neighbor, there's no node between start node and direct node.
     * I will recover this functionality later under churn.
    if (last_node!=NULL) {
      DeetooMessage* m_l = new DeetooMessage(_item, last_node->getAddress(_cache),start->getAddress(_cache)-1,_cache);
      visited_net->add( m_l->visit(last_node, net));
      delete m_l;
    }
    */
    for ( int rgs = 0; rgs < range_low.size() ; rgs++)
    {
	unsigned long int rg_low = range_low.at(rgs).first, rg_up= ( (range_low.at(rgs).second)->getAddress(_cache) );
        DeetooMessage* m_low = new DeetooMessage(_item, rg_low, rg_up, _cache);
	visited_net->add(m_low->visit( (range_low.at(rgs)).second, net) );
	delete m_low;
    }
    range_low.clear();
    //put us in
  
    //visited_net->add(start);
    
    //go to the uppper side.
    //last_lower = start->getAddress(_cache) + 1;
    unsigned long int last_upper = _r1;
    AddressedNode* lastNodeUp = NULL;
    std::vector<pair<unsigned long int, AddressedNode*> > range_up2;
    range_up2.clear();
    std::map<unsigned long int, AddressedNode*>::reverse_iterator it_up2;
    for (it_up2=upper_neighbors.rbegin(); it_up2!=upper_neighbors.rend(); it_up2++)
    {
      if (inRange( it_up2->second) )
      {
          visited_net->add(it_up2->second);
          std::set<std::string> this_itemSet = it_up2->second->getItem();
          if (this_itemSet.find(_item) != this_itemSet.end() ) 
          {
            hit = true;
	    //cout << " =========hit???\t " << hit << endl;
	    break;
	  }
	  //cout << " hit???\t " << hit << endl;
	  range_up2.push_back(make_pair (last_upper, it_up2->second ) );
	  //DeetooMessage* m_up = new DeetooMessage(_item, last_lower, it_up2->first, _cache);
	  //visited_net->add( m_up->visit(it_up2->second, net) );
	  last_upper = it_up2->first -1;
	  last_node = it_up2->second;
	  //delete m_up;
      }	
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
    for ( int rg = 0; rg < range_up2.size() ; rg++)
    {
	//unsigned long int rg_low = range_up2.at(rg).first, rg_up= ( (range_up2.at(rg).second)->getAddress(_cache) );
	unsigned long int rg_low = (range_up2.at(rg).second)->getAddress(_cache), rg_up= range_up2.at(rg).first;
	DeetooMessage* m_up = new DeetooMessage(_item, rg_low, rg_up, _cache);
	visited_net->add(m_up->visit( (range_up2.at(rg)).second, net) );
	delete m_up;
    }
    range_up2.clear();
    lower_neighbors.clear();
    upper_neighbors.clear();
  }
  cout << "complete query... Wow!!!!!!!!!!!!" << endl;
  return visited_net;
}

void DeetooMessage::cacheItems(AddressedNode* start,  DeetooNetwork* net) 
{
  //_cache = true;	
  //AddressedNode* start = dynamic_cast<AddressedNode*> (n);
  AddressedNode* next_node;
  std::map<unsigned long int, AddressedNode*> lower_neighbors;
  std::map<unsigned long int, AddressedNode*> upper_neighbors;
  lower_neighbors.clear();
  upper_neighbors.clear();
  //cout << "start caching" << endl;
  if (!inRange(start) )  //node is not in this range
  {
    //unsigned long int dist_to_lower = 0;
    unsigned long int dist_to_lower = 0;
    auto_ptr<NodeIterator> ni(net->getNeighborIterator(start) );
    //cout << "start iteration for not in range" << endl;
    while (ni->moveNext() )
    {
        AddressedNode* c_node = dynamic_cast<AddressedNode*>(ni->current() );
        if ( ( c_node->getDistanceTo(_r0, _cache) < dist_to_lower) || (dist_to_lower <= 0) )
	{
            next_node = c_node;
	    dist_to_lower = next_node->getDistanceTo(_r0, _cache);
	}
    }
    //We have the closest neighbor to lower, start over there
    cacheItems(next_node, net) ;
  }
  else 
  {
      //We are in the range, get the neighbors which are also in range.
      //divide range to upper and lower.
      //get upper neighbors and lower neighbors.
      //std::map will sort them according to their address, lowest first.
      //cout << "start iteration for dividing neighbors" << endl;
      auto_ptr<NodeIterator> ni(net->getNeighborIterator(start) );
      while(ni->moveNext())
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

  //cout << " lower part first " << endl;
  //Start with lower neighbors first.
  //cout << " lower_neighbor's size: " << lower_neighbors.size() <<endl;
  unsigned long int last_lower = _r0;
  AddressedNode* last_node=NULL;
  //std::vector<pair<unsigned long int, unsigned long int> > ranges1;
  std::vector<pair<unsigned long int, AddressedNode*> > ranges1;
  ranges1.clear();
  //cout << "\t navigate lower part first" << endl;
  std::map<unsigned long int, AddressedNode*>::iterator it_low;
  for (it_low=lower_neighbors.begin(); it_low!=lower_neighbors.end(); it_low++)
  { 
      if (inRange( it_low->second) )
      {
        //visited_net->add(it_low->second);
        (it_low->second)->insertItem(_item);
	ranges1.push_back(make_pair ( last_lower, it_low->second ) );
	//DeetooMessage* m_low = new DeetooMessage(_item, last_lower, it_low->first, _cache);
	//visited_net->add( m_low->visit(it_low->second, net) );
	last_lower = it_low->first + 1;
	assert(last_lower>=0);
	last_node = it_low->second;
	//cout << "last_node: " << last_node->getAddress(_cache) << endl;
	assert(last_node>=0);
	//delete m_low;
      }
		
  }
  /**
   * I ignore the last bit since its direct neighbor, there's no node between start node and direct node.
   * I will recover this functionality later under churn.
  if (last_node!=NULL) {
    cout << "================-----------------============================= " << endl;
    //cout << "start node's addr: " << start->getAddress(_cache) << endl;
    //cout << "last node's addr: " << last_node->getAddress(_cache) << endl;
    DeetooMessage* m_l = new DeetooMessage(_item, last_node->getAddress(_cache), (start->getAddress(_cache)-1),_cache);
    cout << "================================================== " << endl;
    //visited_net->add( m_l->visit(last_node, net));
    delete m_l;
  }
  */
  for ( int rgs = 0; rgs < ranges1.size(); rgs++)
  {
      unsigned long int rg_low = ranges1.at(rgs).first, rg_up= ( (ranges1.at(rgs).second)->getAddress(_cache) );
      DeetooMessage* m_low = new DeetooMessage(_item, rg_low, rg_up, _cache);
      //visited_net->add(m_low->visit( (ranges1.at(rgs)).second, net) );
      m_low->cacheItems( (ranges1.at(rgs)).second, net) ;
      delete m_low;
  }
  ranges1.clear();
  //delete last_node;
  //delete m_l;
  
  /*************************************************************/  
  //cout << "Move to the upper part" << endl;
  //go to the uppper side.
  //last_lower = start->getAddress(_cache) + 1;
  unsigned long int last_upper = _r1;
  AddressedNode* lastNodeUp = NULL;
  std::vector<pair<unsigned long int, AddressedNode*> > range_up;
  range_up.clear();
  std::map<unsigned long int, AddressedNode*>::reverse_iterator it_up;
  for (it_up=upper_neighbors.rbegin(); it_up!=upper_neighbors.rend(); it_up++)
  {
    if (inRange( it_up->second ) )
    {
        //visited_net->add(it_up->second);
	it_up->second->insertItem(_item);
	//range_up.push_back(make_pair ( last_lower, it_up->second) );
	range_up.push_back(make_pair ( last_upper, it_up->second) );
	//DeetooMessage* m_up = new DeetooMessage(_item, last_lower, it_up->first, _cache);
	//visited_net->add( m_up->visit(it_up->second, net) );
	last_upper = it_up->first -1;
	lastNodeUp = it_up->second;
	//delete m_up;
    }	
  }
  /**
   * I ignore the last bit since its direct neighbor, there's no node between start node and direct node.
   * I will recover this functionality later under churn.
  if (lastNodeUp!=NULL) {
      DeetooMessage* m_u = new DeetooMessage(_item, (lastNodeUp->getAddress(_cache)+1),_r1, _cache);
      visited_net->add( m_u->visit(lastNodeUp, net));
      delete m_u;
  }
  */
  for ( int rg = 0; rg < range_up.size(); rg++)
  {
      //unsigned long int rg_low = range_up.at(rg).first, rg_up= ( (range_up.at(rg).second)->getAddress(_cache) );
      unsigned long int rg_low = (range_up.at(rg).second)->getAddress(_cache), rg_up= range_up.at(rg).first;
      DeetooMessage* m_up = new DeetooMessage(_item, rg_low, rg_up, _cache);
      //visited_net->add(m_up->visit( (range_up.at(rg)).second, net) );
      m_up->cacheItems( (range_up.at(rg)).second, net) ;
      delete m_up;
  }
  //delete lastNodeUp;
  //delete next_node;
  range_up.clear();
  lower_neighbors.clear();
  upper_neighbors.clear();
  //cout << "complete cache... Wow!!!!!!!!!!!!" << endl;
  //return net;
}
