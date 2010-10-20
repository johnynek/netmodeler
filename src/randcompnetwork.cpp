/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2010  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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

#include "randcompnetwork.h"
#include "containernode.h"
#include "infocom.h"

using namespace Starsky;
using namespace std;

RandCompNetwork::RandCompNetwork(const vector<int>& cnt,
                                 double exp_deg, double outp_inp_ratio,
			         Random& rand) : Network() {
  //Initialize the cluster number ptrs:
  _clusters = new int[cnt.size()];
  int* p_cluster = _clusters;
  auto_ptr< vector< cnt_ptr<Network> > > part_v(new vector< cnt_ptr<Network> >());
  //Make the nodes first:
  for(int cluster = 0; cluster < cnt.size(); cluster++) {
    *p_cluster = cluster;
    //Use
    int cluster_count = cnt[cluster];
    cnt_ptr<Network> cluster_net(new Network());
    part_v->push_back(cluster_net);
    for(int node_c = 0; node_c < cluster_count; node_c++) {
       ContainerNode<int>* this_node = new ContainerNode<int>(p_cluster, false);
       add(this_node);
       cluster_net->add(this_node);
    }
    //Use ptr arithmetic here
    p_cluster++;
  }
  //We have made all the nodes
  auto_ptr<NodeIterator> ni(getNodeIterator());
  int ntot = getNodeSize();
  while(ni->moveNext()) {
    ContainerNode<int>* n1 = dynamic_cast<ContainerNode<int>*>(ni->current());
    int* c1p = n1->get();
    //cout << "c1p: " << c1p << " cluster: " << *c1p << endl;
    int n_c = cnt[*c1p]; //Nodes in this cluster
    //cout << "cnt: " << cnt[*c1p] << endl;
    double pc = getInCompProb(exp_deg, n_c, ntot, outp_inp_ratio);
    double pcbar = pc * outp_inp_ratio;
    //cout << "pc = " << pc << " pcbar = " << pcbar << endl;
    auto_ptr<NodeIterator> nj(ni->clone());
    while(nj->moveNext()) {
      ContainerNode<int>* n2 = dynamic_cast<ContainerNode<int>*>(nj->current());
      double p = ( c1p == n2->get() ) ? pc : pcbar;
      if( rand.getBool(p) ) {
        add( Edge(n1,n2) );
      }
    }
  }
  //Put in the joining edges
  for(vector< cnt_ptr<Network> >::iterator it = part_v->begin();
      it != part_v->end(); it++) {
    (*it)->addJoiningEdgesFrom(this);
  }
  _part = new NetworkPartition(*this, part_v.release());
}
RandCompNetwork::~RandCompNetwork() {
  delete[] _clusters;
}

double RandCompNetwork::accuracyOfPartition(NetworkPartition* part) const {
  auto_ptr<EdgeIterator> ei(getEdgeIterator());
  std::set<Edge*> in_cluster;
  while(ei->moveNext()) {
    Edge* e = ei->current();
    ContainerNode<int>* nf = dynamic_cast<ContainerNode<int>*>(e->first);
    ContainerNode<int>* ns = dynamic_cast<ContainerNode<int>*>(e->second);
    if( nf->get() == ns->get() ) {
      //These nodes are in the same cluster:
      in_cluster.insert(e);
    }
  }
  long long correct_internal = 0;
  long long incorrect_internal = 0;
  auto_ptr<Iterator<cnt_ptr<Network> > > it(part->getComponents());
  while(it->moveNext()) {
    const cnt_ptr<Network>& this_net = it->current();
    auto_ptr<EdgeIterator> tei(this_net->getEdgeIterator());
    while(tei->moveNext()) {
      Edge* this_edge = tei->current();
      std::set<Edge*>::iterator sit = in_cluster.find(this_edge);
      if( sit != in_cluster.end() ) {
        //we correctly identified this edge as a "internal edge"
        correct_internal++;
        in_cluster.erase(sit);
      }
      else {
        //We incorrectly 
        incorrect_internal++;
      }
    }
  }
  //Any still in the in_cluster were the ones we thought were external, but really internal
  long long incorrect_external = in_cluster.size();
  long long total_edges = getEdgeSize();
  /*
   * correct_interal + correct_external + incorrect_internal + incorrect_ext = total
   * correct = correct_int + correct_ex
   * accuracy = correct/total
   * correct_ex = total - correct_int - incorrect_int - incorrect_ext
   */
  long long correct = total_edges - incorrect_external - incorrect_internal;
  return (double)correct/(double)total_edges;
}

double RandCompNetwork::informationOfPartition(NetworkPartition* part) const {
  auto_ptr<EdgeIterator> ei(getEdgeIterator());
  std::set<Edge*> in_cluster;
  while(ei->moveNext()) {
    Edge* e = ei->current();
    ContainerNode<int>* nf = dynamic_cast<ContainerNode<int>*>(e->first);
    ContainerNode<int>* ns = dynamic_cast<ContainerNode<int>*>(e->second);
    if( nf->get() == ns->get() ) {
      //These nodes are in the same cluster:
      in_cluster.insert(e);
    }
  }
  long long total_internal = in_cluster.size();
  long long correct_internal = 0;
  long long incorrect_internal = 0;
  auto_ptr<Iterator<cnt_ptr<Network> > > it(part->getComponents());
  while(it->moveNext()) {
    const cnt_ptr<Network>& this_net = it->current();
    auto_ptr<EdgeIterator> tei(this_net->getEdgeIterator());
    while(tei->moveNext()) {
      Edge* this_edge = tei->current();
      std::set<Edge*>::iterator sit = in_cluster.find(this_edge);
      if( sit != in_cluster.end() ) {
        //we correctly identified this edge as a "internal edge"
        correct_internal++;
        in_cluster.erase(sit);
      }
      else {
        //We incorrectly 
        incorrect_internal++;
      }
    }
  }
  //Any still in the in_cluster were the ones we thought were external, but really internal
  long long incorrect_external = in_cluster.size();
  long long total_edges = getEdgeSize();
  long long total_external = total_edges - total_internal;
  long long correct_external = total_edges - correct_internal
                                           - incorrect_internal
                                           - incorrect_external;
  double p_int = (double)total_internal/(double)total_edges;
  double p_ext = (double)total_external/(double)total_edges;
  double p_Igi = (double)correct_internal/(double)total_internal;
  double p_Ege = (double)correct_external/(double)total_external;
  //cout << "p_int: " << p_int << " p_ext: " << p_ext << " p_Igi: " << p_Igi << " p_Ege: " << p_Ege << endl;
  double p_I = (double)(correct_internal + incorrect_internal)/(double)total_edges;
  //cout << "p_I = " << p_I << endl;
  if( p_int <= 0.0 ) {
    if( p_I <= 0.0 ) {
      return 1.0;
    }
    return 0.0;
  } 
  if( p_int >= 1.0 ) {
    if( p_I >= 1.0 ) {
      return 1.0;
    }
    return 0.0;
  }
  else {
    return (InfoCom::h2(p_I) - (p_int*InfoCom::h2(p_Igi) + p_ext*InfoCom::h2(1.0 - p_Ege)))
           /InfoCom::h2(p_int);
  }
}

double RandCompNetwork::getInCompProb(double d, int n_c,
                                      int ntot, double r) const {
  double pc = d/( (n_c - 1) + (ntot - 1 - (n_c - 1)) * r);
  if( pc > 1.0 ) {
    cerr << "Cluster edge probability greater than one: " << pc << endl;
    pc = 1.0;
  }
  return pc;
}
