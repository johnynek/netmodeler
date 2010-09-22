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

#include "inetworkpartitioner.h"

#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)
#define FOREACHp(it, col) for(it = col->begin(); it != col->end(); it++)

using namespace std;
using namespace Starsky;

NetworkPartition::NetworkPartition(const Network& orig, vector<Network*>* part) :
  _orig(orig), _part(part), _part_as_net(0) { }

NetworkPartition::~NetworkPartition() {
  
  vector<Network*>::iterator it;
  for(it = _part->begin(); it != _part->end(); it++)
  {
    //Delete the Network:
    if( *it != 0 ) {
      delete *it;
    }
  }
  delete _part;
  if( _part_as_net != 0 ) {
    delete _part_as_net;
    _part_as_net = 0;
  }
}

const Network& NetworkPartition::asNetwork() {
  if( _part_as_net == 0 ) {
    Network* result = new Network();
    std::map<Node*, Network*> node_to_part;
    std::map<Network*, ContainerNode<Network>* > part_to_node;
    ContainerIterator<std::vector<Network*> > partit(*_part);
    //Add the nodes of the network:
    while( partit.moveNext() ) {
      Network* temp_net = partit.current();
      std::auto_ptr<NodeIterator> ni( temp_net->getNodeIterator() );
      while( ni->moveNext() ) {
        node_to_part[ ni->current() ] = temp_net;
      }
      //Make a new network, but don't own this network
      ContainerNode<Network>* n = new ContainerNode<Network>( temp_net, false );
      part_to_node[ temp_net ] = n;
      result->add(n);
    }
    //Now we add the edges, which includes all the edges that go between nodes:
    std::auto_ptr<EdgeIterator> ei( _orig.getEdgeIterator() );
    while( ei->moveNext() ) {
      Edge* this_edge = ei->current();
      Node* a = this_edge->first;
      Node* b = this_edge->second;
      Network* neta = node_to_part[a];
      Network* netb = node_to_part[b];
      if( neta != netb ) {
        //These two nodes are not in the same component
        Node* nneta = part_to_node[neta];
        Node* nnetb = part_to_node[netb];
        Edge* temp_e = result->getEdge(nneta, nnetb);
        if( temp_e == 0 ) {
          //We are making a new edge:
  	Network* new_net = new Network();
  	temp_e = new ContainerEdge<Network>(nneta, nnetb, new_net);
          result->add(temp_e);
        }
        //There is already an edge:
        ContainerEdge<Network>* ce = dynamic_cast<ContainerEdge<Network>* >(temp_e);
        Network* con_net = ce->get();
        con_net->add( this_edge );
      }
    }
    _part_as_net = result;
  }
  return *_part_as_net;
}

Iterator<Network*>* NetworkPartition::getComponents() const
{
  return new ContainerIterator<vector<Network*> >(*_part);
}

double NetworkPartition::getModularity() const
{
  //Now net_vec indexes the communities
  map<Node*, Network*> node_community;
  vector<Network*>::const_iterator netit;
  for(netit = _part->begin(); netit != _part->end(); netit++)
  {
    auto_ptr<NodeIterator> ni( (*netit)->getNodeIterator() );
    while( ni->moveNext() ) {
      node_community[ ni->current() ] = *netit;
    }
  }
  
  map<Network*, map<Network*, double> > e_ij;
  Network *com1, *com2;
  double e_total = 0.0;
  auto_ptr<EdgeIterator> ei( _orig.getEdgeIterator() );
  while( ei->moveNext() ) {
    Edge* this_edge = ei->current();
    com1 = node_community[ this_edge->first ];
    com2 = node_community[ this_edge->second ];
    e_ij[com1][com2] += 1.0;
    e_ij[com2][com1] += 1.0;
    e_total += 2.0;
  }
  //Normalize e_ij:
  vector<Network*>::const_iterator netit2;
  for(netit = _part->begin(); netit != _part->end(); netit++)
  {
    Network* ni = *netit;
    for(netit2 = _part->begin(); netit2 != _part->end(); netit2++)
    {
      Network* nj = *netit2;
      e_ij[ni][nj] /= e_total;
    }
  }
  //Make a_i;
  map<Network*, double> a_i;
  for(netit = _part->begin(); netit != _part->end(); netit++)
  {
    Network* ni = *netit;
    a_i[ni] = 0.0;
    for(netit2 = _part->begin(); netit2 != _part->end(); netit2++)
    {
      Network* nj = *netit2;
      a_i[ni] += e_ij[ni][nj];
    }
  }
  //We don't neccesarily start at Q=0;
  double q = 0.0;
  //for(int i = 0; i < e_ij.size(); i++) {
  for(netit = _part->begin(); netit != _part->end(); netit++)
  {
    Network* i = *netit;
    q += e_ij[i][i] - a_i[i] * a_i[i];
  }
  
  return q;
	
}

long INetworkPartitioner::distance(std::vector<Network*>* A,
		                   std::vector<Network*>* B,
		        long& norm_a, long& norm_b)
{
  //First we make some data structures:
  map<Node*, Network*> a_map, b_map;
  vector<Network*>::iterator nit;
  Network all_nodes;
  //Make the a_map
  FOREACH(nit, (*A)) {
    auto_ptr<NodeIterator> ni( (*nit)->getNodeIterator() );
    while(ni->moveNext()) {
      Node* this_node = ni->current();
      a_map[ this_node ] = *nit;
      all_nodes.add(this_node);
    }
  }
  //Make the b_map
  FOREACH(nit, (*B)) {
    auto_ptr<NodeIterator> ni( (*nit)->getNodeIterator() );
    while(ni->moveNext()) {
      Node* this_node = ni->current();
      b_map[ this_node ] = *nit;
      all_nodes.add(this_node);
    }
  }
  //We need a vector to make the algorithm run faster:
  vector<Node*> n_vec;
  auto_ptr<NodeIterator> ni( all_nodes.getNodeIterator() );
  n_vec.resize( all_nodes.getNodeSize() );
  ni->pushInto(n_vec);
  //Get rid of the memory:
  all_nodes.clear();

  norm_a = 0;
  norm_b = 0;
  long dist = 0;
  bool a_has, b_has;
  map<Node*, Network*>::iterator mapita, mapitb;
  //Now we can index the nodes by integers:
  for(int i = 0; i < n_vec.size(); i++) {
    for(int j = i+1; j < n_vec.size(); j++) {
      mapita = a_map.find( n_vec[i] );
      if( mapita != a_map.end() ) {
        Network* n = mapita->second;
        a_has = n->has( n_vec[j] );
      }
      mapitb = b_map.find( n_vec[i] );
      if( mapitb != b_map.end() ) {
        Network* n = mapitb->second;
        b_has = n->has( n_vec[j] );
      }
      if( a_has ) {
        norm_a++;
      } 
      if( b_has ) {
        norm_b++;
      }
      if( a_has == b_has ) {
        // dist = dist;
      }
      else {
        //One has it, the other doesn't
        dist++;
      }
    }
  }
  return dist;
}
