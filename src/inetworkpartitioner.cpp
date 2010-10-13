/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005-2010  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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

NetworkPartition::NetworkPartition(const Network& orig, vector<cnt_ptr<Network> >* part) :
  _orig(orig), _part(part), _part_as_net(0) {
}

const Network& NetworkPartition::asNetwork() {
  if( _part_as_net == 0 ) {
    Network* result = _orig.newNetwork();
    std::map<Node*, Network* > node_to_part;
    std::map<Network*, ContainerNode<Network>* > part_to_node;
    ContainerIterator<std::vector<cnt_ptr<Network> > > partit(*_part);
    //Add the nodes of the network:
    while( partit.moveNext() ) {
      const cnt_ptr<Network>& temp_net = partit.current();
      std::auto_ptr<NodeIterator> ni( temp_net->getNodeIterator() );
      while( ni->moveNext() ) {
        node_to_part[ ni->current() ] = temp_net.get();
      }
      //Make a new network, but don't own this network
      ContainerNode<Network>* n = new ContainerNode<Network>( temp_net.get(), false );
      part_to_node[ temp_net.get() ] = n;
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

map<Node*, cnt_ptr<Network> >* NetworkPartition::asMap() const
{
  map<Node*,cnt_ptr<Network> >* result = new map<Node*, cnt_ptr<Network> >();
  auto_ptr<Iterator<cnt_ptr<Network> > > compit(getComponents());
  while(compit->moveNext()) {
    const cnt_ptr<Network>& this_net = compit->current();
    auto_ptr<Iterator<Node*> > nodeit(this_net->getNodeIterator());
    while(nodeit->moveNext()) {
      Node* this_node = nodeit->current();
      (*result)[this_node] = this_net;
    }
  }
  return result;
}

Iterator<cnt_ptr<Network> >* NetworkPartition::getComponents() const
{
  return new ContainerIterator<vector<cnt_ptr<Network> > >(*_part);
}

cnt_ptr<Network> NetworkPartition::getComponentOf(Node* node) const {
  auto_ptr<Iterator<cnt_ptr<Network> > > compit(getComponents());
  while(compit->moveNext()) {
    const cnt_ptr<Network>& this_net = compit->current();
    if(this_net->has(node)) {
      return this_net;
    }
  }
  cout << "Couldn't find: " << node << endl;
  return cnt_ptr<Network>(0);
}

double NetworkPartition::getModularity() const
{
  //Now net_vec indexes the communities
  auto_ptr<map<Node*, cnt_ptr<Network> > > node_community(asMap());
  
  map<Network*, map<Network*, double> > e_ij;
  Network *com1, *com2;
  double e_total = 0.0;
  auto_ptr<EdgeIterator> ei( _orig.getEdgeIterator() );
  while( ei->moveNext() ) {
    Edge* this_edge = ei->current();
    com1 = ((*node_community)[ this_edge->first ]).get();
    com2 = ((*node_community)[ this_edge->second ]).get();
    e_ij[com1][com2] += 1.0;
    e_ij[com2][com1] += 1.0;
    e_total += 2.0;
  }
  //Normalize e_ij:
  vector<cnt_ptr<Network> >::const_iterator netit, netit2;
  for(netit = _part->begin(); netit != _part->end(); netit++)
  {
    Network* ni = netit->get();
    for(netit2 = _part->begin(); netit2 != _part->end(); netit2++)
    {
      Network* nj = netit2->get();
      e_ij[ni][nj] /= e_total;
    }
  }
  //Make a_i;
  map<Network*, double> a_i;
  for(netit = _part->begin(); netit != _part->end(); netit++)
  {
    Network* ni = netit->get();
    a_i[ni] = 0.0;
    for(netit2 = _part->begin(); netit2 != _part->end(); netit2++)
    {
      Network* nj = netit2->get();
      a_i[ni] += e_ij[ni][nj];
    }
  }
  //We don't neccesarily start at Q=0;
  double q = 0.0;
  //for(int i = 0; i < e_ij.size(); i++) {
  for(netit = _part->begin(); netit != _part->end(); netit++)
  {
    Network* i = netit->get();
    double this_ai = a_i[i];
    q += e_ij[i][i] - this_ai * this_ai;
  }
  
  return q;
	
}

double NetworkPartition::getComInformation() const
{
  //Now net_vec indexes the communities
  auto_ptr<map<Node*, cnt_ptr<Network> > > node_community(asMap());
  
  map<Network*, map<Network*, double> > e_ij;
  Network *com1, *com2;
  double e_total = 0.0;
  auto_ptr<EdgeIterator> ei( _orig.getEdgeIterator() );
  while( ei->moveNext() ) {
    Edge* this_edge = ei->current();
    com1 = ((*node_community)[ this_edge->first ]).get();
    com2 = ((*node_community)[ this_edge->second ]).get();
    e_ij[com1][com2] += 1.0;
    e_ij[com2][com1] += 1.0;
    e_total += 2.0;
  }
  //Normalize e_ij:
  vector<cnt_ptr<Network> >::const_iterator netit, netit2;
  for(netit = _part->begin(); netit != _part->end(); netit++)
  {
    Network* ni = netit->get();
    for(netit2 = _part->begin(); netit2 != _part->end(); netit2++)
    {
      Network* nj = netit2->get();
      e_ij[ni][nj] /= e_total;
    }
  }
  //Make a_i;
  map<Network*, double> a_i;
  for(netit = _part->begin(); netit != _part->end(); netit++)
  {
    Network* ni = netit->get();
    a_i[ni] = 0.0;
    for(netit2 = _part->begin(); netit2 != _part->end(); netit2++)
    {
      Network* nj = netit2->get();
      a_i[ni] += e_ij[ni][nj];
    }
  }
  /*
   * Treat e_ij like a symmetric probability distribution,
   * So, I = H(A) + H(A) - H(E) 
   */
  double HA = 0.0;
  for(map<Network*, double>::const_iterator ait = a_i.begin();
      ait != a_i.end(); ait++) {
    double this_ai = ait->second;
    if( this_ai > 0 ) {
      HA -= this_ai * log(this_ai);
    }
  }
  HA = HA / log(2.0); //Normalize to log base 2
  
  double HE = 0.0;
  for(map<Network*, map<Network*, double> >::const_iterator eit1 = e_ij.begin();
      eit1 != e_ij.end(); eit1++) {
    const map<Network*, double>& ei = eit1->second;
    for(map<Network*, double>::const_iterator eit2 = ei.begin();
        eit2 != ei.end(); eit2++) {
      double this_eij = eit2->second;
      if( this_eij > 0 ) {
        HE -= this_eij * log(this_eij);
      }
    }
  }
  HE = HE / log(2.0);
  return HA + HA - HE;
}

int total_nodes(vector<cnt_ptr<Network> >* part) {
  int total = 0;
  for(vector<cnt_ptr<Network> >::const_iterator it = part->begin();
      it != part->end(); it++) {
    int this_size = (*it)->getNodeSize();
    if( this_size == 0 ) { cout << "Zero sized component" << endl; }
    total += this_size;
  }
  return total;
}

void NetworkPartition::insert(vector<cnt_ptr<Network> >* part, const cnt_ptr<Network>& n) {
  ptr_gt<cnt_ptr<Network> > comp;
  vector<cnt_ptr<Network> >::iterator pos = lower_bound(part->begin(), part->end(), n, comp);
  //Debug:
  int t = total_nodes(part);  
  part->insert(pos, n);
  if( t + n->getNodeSize() != total_nodes(part) ) {
    cout << "Failed to increase size in insert" << endl;
  }
}

NetworkPartition* NetworkPartition::isolate(Node* n) const {
  cnt_ptr<Network> com = getComponentOf(n);
  if( com.isNull() ) {
    //No such network
    return 0; 
  }
  if( com->getNodeSize() == 1 ) {
    //Already isolated
    return 0; 
  }
  else {
    //Copy the partition definition:
    vector<cnt_ptr<Network> >* new_part = new vector<cnt_ptr<Network> >(*_part);
    //Here is the new isolated component
    cnt_ptr<Network> new_net( com->newNetwork() );
    new_net->add(n);
    //Create a copy of the old, without n
    cnt_ptr<Network> new_com( com->clone() );
    new_com->remove(n);

    NetworkPartition::replace(new_part, com, new_com);
    //Add the new partition: 
    NetworkPartition::insert(new_part, new_net);
    if( total_nodes(_part.get()) != total_nodes(new_part) ) {
      cout << "started: " << total_nodes(_part.get())
           << " now: " << total_nodes(new_part) << endl;
    }
    return new NetworkPartition(_orig, new_part); 
  }
}

NetworkPartition* NetworkPartition::join(const cnt_ptr<Network>& com1, const cnt_ptr<Network>& com2) const {
  if( com1 == com2 ) {
    //Join doesn't make sense
    return 0;
  }
  //Copy
  vector<cnt_ptr<Network> >* new_part = new vector<cnt_ptr<Network> >(*_part);
  NetworkPartition::remove(new_part, com1);
  NetworkPartition::remove(new_part, com2);
  
  //Create the new joined network:
  cnt_ptr<Network> new_net( com1->clone() );
  auto_ptr<Iterator<Node*> > ni(com2->getNodeIterator());
  while(ni->moveNext()) { new_net->add(ni->current()); }
  new_net->addJoiningEdgesFrom(&_orig);
  
  NetworkPartition::insert(new_part, new_net);
  return new NetworkPartition(_orig, new_part);
}

NetworkPartition* NetworkPartition::transfer(const cnt_ptr<Network>& com, Edge* edge) const {
  cnt_ptr<Network> n1 = getComponentOf(edge->first);
  cnt_ptr<Network> n2 = getComponentOf(edge->second);
  if( (n1 == n2) && (com == n1) ) {
    //This transfer is meaningless
    return 0;
  }
  vector<cnt_ptr<Network> >* new_part = new vector<cnt_ptr<Network> >(*_part);
  cnt_ptr<Network> new_com( com->clone() );
  new_com->add(edge);
  new_com->addJoiningEdgesFrom((Network*)&_orig);
  NetworkPartition::replace(new_part, com, new_com);

  if( n1 != com ) {
    if( n1->getNodeSize() > 1 ) {
      //first wasn't already in com
      cnt_ptr<Network> new_n1 = n1->clone();
      new_n1->remove(edge->first);
      NetworkPartition::replace(new_part, n1, new_n1);
    }
    else {
      //We just removed the last node
      NetworkPartition::remove(new_part, n1);
    }
    if( n2 != com ) {
      cout << "both n1 != com and (n2 != com)" << endl;
      cout << " n1: " << n1.get()
           << " n2: " << n2.get()
           << " com: " << com.get()
           << endl;
    }
  }
  if( n2 != com ) {
    if( n2->getNodeSize() > 1 ) {
      //second wasn't already in com
      cnt_ptr<Network> new_n2 = n2->clone();
      new_n2->remove(edge->second);
      NetworkPartition::replace(new_part, n2, new_n2);
    }
    else {
      //We just removed the last node
      NetworkPartition::remove(new_part, n2);
    }
    if( n1 != com ) {
      cout << "both (n1 != com) and n2 != com" << endl;
      cout << " n1: " << n1.get()
           << " n2: " << n2.get()
           << " com: " << com.get()
           << endl;
   
    }
  }
    if( total_nodes(_part.get()) != total_nodes(new_part) ) {
      cout << "started: " << total_nodes(_part.get())
           << " now: " << total_nodes(new_part) << endl;
    }
  return new NetworkPartition(_orig, new_part); 
}


bool NetworkPartition::remove(vector<cnt_ptr<Network> >* part, const cnt_ptr<Network>& com) {
  ptr_gt<cnt_ptr<Network> > comp;
  vector<cnt_ptr<Network> >::iterator pos = lower_bound(part->begin(), part->end(), com, comp);
  if( *pos == com ) {
    //The item is there, erase it:
    part->erase(pos);
    return true;
  }
  return false;
}

bool NetworkPartition::replace(vector<cnt_ptr<Network> >* part,
                               const cnt_ptr<Network>& com1,
                               const cnt_ptr<Network>& com2) {
  ptr_gt<cnt_ptr<Network> > comp;
  vector<cnt_ptr<Network> >::iterator pos = lower_bound(part->begin(), part->end(), com1, comp);
  if( *pos == com1 ) {
    //The item is there:
    vector<cnt_ptr<Network> >::iterator pos2 = lower_bound(part->begin(), part->end(), com2, comp);
    if( pos == pos2 ) {
      //We can just overwrite:
      *pos = com2;
    }
    else {
      //Erase then insert:
      part->erase(pos);
      NetworkPartition::insert(part, com2);
    }
    return true;
  }
  else {
    //com1 wasn't there, but put com2 in:
    NetworkPartition::insert(part, com2);
    return false;
  } 
}

int NetworkPartition::size() const {
  return _part->size();
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
