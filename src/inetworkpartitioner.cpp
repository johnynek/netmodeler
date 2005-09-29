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

void INetworkPartitioner::deletePartition(set<Network*>* part)
{
  set<Network*>::iterator it;
  for(it = part->begin(); it != part->end(); it++)
  {
    //Delete the Network:
    delete *it;
  }
  //Delete the Set:
  delete part;
}

Network* INetworkPartitioner::getLargest(set<Network*>* part) {

  set<Network*>::const_iterator it;
  Network* ret_val = 0;
  FOREACH(it, (*part))
  {
    Network* this_net = *it;
    networkptr_gt cmp;
    if( (ret_val == 0) || cmp(this_net, ret_val) ) {
      ret_val = this_net;
    }
  }
  return ret_val;
}

double INetworkPartitioner::modularityOf(set<Network*>* partition,
		                         const Network& orig)
{

  //Now net_vec indexes the communities
  map<Node*, Network*> node_community;
  set<Network*>::const_iterator netit;
  for(netit = partition->begin(); netit != partition->end(); netit++)
  {
    NodeIterator ni = (*netit)->getNodeIterator();
    while( ni.moveNext() ) {
      node_community[ ni.current() ] = *netit;
    }
  }
  
  map<Network*, map<Network*, double> > e_ij;
  Network *com1, *com2;
  double e_total = 0.0;
  EdgeIterator ei = orig.getEdgeIterator();
  while( ei.moveNext() ) {
    Edge* this_edge = ei.current();
    com1 = node_community[ this_edge->first ];
    com2 = node_community[ this_edge->second ];
    e_ij[com1][com2] += 1.0;
    e_ij[com2][com1] += 1.0;
    e_total += 2.0;
  }
  //Normalize e_ij:
  set<Network*>::const_iterator netit2;
  for(netit = partition->begin(); netit != partition->end(); netit++)
  {
    Network* ni = *netit;
    for(netit2 = partition->begin(); netit2 != partition->end(); netit2++)
    {
      Network* nj = *netit2;
      e_ij[ni][nj] /= e_total;
    }
  }
  //Make a_i;
  map<Network*, double> a_i;
  for(netit = partition->begin(); netit != partition->end(); netit++)
  {
    Network* ni = *netit;
    a_i[ni] = 0.0;
    for(netit2 = partition->begin(); netit2 != partition->end(); netit2++)
    {
      Network* nj = *netit2;
      a_i[ni] += e_ij[ni][nj];
    }
  }
  //We don't neccesarily start at Q=0;
  double q = 0.0;
  //for(int i = 0; i < e_ij.size(); i++) {
  for(netit = partition->begin(); netit != partition->end(); netit++)
  {
    Network* i = *netit;
    q += e_ij[i][i] - a_i[i] * a_i[i];
  }
  
  return q;
	
}

long INetworkPartitioner::distance(std::set<Network*>* A, std::set<Network*>* B,
		        long& norm_a, long& norm_b)
{
  //First we make some data structures:
  map<Node*, Network*> a_map, b_map;
  set<Network*>::iterator nit;
  Network::NodePSet all_nodes;
  //Make the a_map
  FOREACH(nit, (*A)) {
    NodeIterator ni = (*nit)->getNodeIterator();
    while(ni.moveNext()) {
      Node* this_node = ni.current();
      a_map[ this_node ] = *nit;
      all_nodes.insert(this_node);
    }
  }
  //Make the b_map
  FOREACH(nit, (*B)) {
    NodeIterator ni = (*nit)->getNodeIterator();
    while(ni.moveNext()) {
      Node* this_node = ni.current();
      b_map[ this_node ] = *nit;
      all_nodes.insert(this_node);
    }
  }
  //We need a vector to make the algorithm run faster:
  vector<Node*> n_vec;
  n_vec.insert( n_vec.begin(), all_nodes.begin(), all_nodes.end() );
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
