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

#include "newmancom.h"
#include <memory>

using namespace std;
using namespace Starsky;

AgglomPart::AgglomPart() {
  _weighted = false;
}

NetworkPartition* AgglomPart::partition(const Network& input) {

  vector<double> q_t;
  vector< pair<int,int> > joins;
  int best_join = getCommunities(input, q_t, joins);
  return getCommunity(input, best_join, joins);	
}

int AgglomPart::getCommunities(const Network& net, std::vector<double>& q_t,
		   std::vector< std::pair<int,int> >& joins) 
{
  //Which community does a given node belong to:
  map<Node*, int> node_community;

  //Initialize everything here:
  Network::NodePSet::const_iterator n_it;
  int community = 0;
  auto_ptr<NodeIterator> ni( net.getNodeIterator() );
  while( ni->moveNext() ) {
    node_community[ ni->current() ] = community;
    community++;
  }
  int com1, com2;
  double e_total = 0.0;
  //This is the only difference in the wieghted version of the algorithm:
  std::map<int, std::map<int, double> > e_ij;
  if( _weighted == false ) {
    auto_ptr<EdgeIterator> ei( net.getEdgeIterator() );
    while( ei->moveNext() ) {
      Edge* this_edge = ei->current();
      com1 = node_community[ (this_edge)->first ];
      com2 = node_community[ (this_edge)->second ];
      e_ij[com1][com2] += 1.0;
      e_ij[com2][com1] += 1.0;
      e_total += 2.0;
    }
  }
  else {
    auto_ptr<EdgeIterator> ei( net.getEdgeIterator() );
    while( ei->moveNext() ) {
      Edge* this_edge = ei->current();
      double weight = (this_edge)->getWeight();
      com1 = node_community[ (this_edge)->first ];
      com2 = node_community[ (this_edge)->second ];
      e_ij[com1][com2] += weight;
      e_ij[com2][com1] += weight;
      e_total += weight + weight;
    }
  }
  //Normalize e_ij:
  std::map<int, std::map<int, double> >::iterator it1;
  std::map<int, double>::iterator it2;
  //Make a_i;
  vector<double> a_i;
  a_i.resize( net.getNodeSize() );
  
  for(it1 = e_ij.begin(); it1 != e_ij.end(); it1++) {
    for(it2 = it1->second.begin();
        it2 != it1->second.end();
	it2++) {
	it2->second /= e_total;
    }
  }
  for(it1 = e_ij.begin(); it1 != e_ij.end(); it1++) {
    a_i[it1->first] = 0.0;
    for(it2 = it1->second.begin();
        it2 != it1->second.end();
	it2++) {
      a_i[it1->first] += it2->second;
    }
  }
  joins.clear();
  joins.resize( net.getNodeSize() - 1 );
  q_t.clear();
  q_t.resize( net.getNodeSize() );
  //We don't neccesarily start at Q=0;
  double q = 0.0;
  for(it1 = e_ij.begin(); it1 != e_ij.end(); it1++) {
    int i = it1->first;
    q += e_ij[i][i] - a_i[i] * a_i[i];
  }
  q_t[0] = q;
  bool got_first = false;
  double delta_q, tmp_delta, q_max;
  int join1, join2, step, step_max;
  step = 0;
  step_max = step;
  q_max = q;
  //Begin the algorithm:
  for( int k = 0; k < joins.size(); k++) {
    //If we do no joins, there is delta_q = 0.0
    delta_q = 0.0;
#if 0
    //These are not valid communities, but initialize to this:
    com1 = -1;
    com2 = -1;
    //Iterate over each edge and find the biggest increase in Q
    got_first = false;
    for(e_it = edge_set.begin();
        e_it != edge_set.end();
        e_it++) {
      com1 = node_community[ (*e_it)->first ];
      com2 = node_community[ (*e_it)->second ];
      if( com1 != com2 ) {
        tmp_delta = 2 * (e_ij[com1][com2] - a_i[com1] * a_i[com2] );
        if( (!got_first) || ( tmp_delta > delta_q ) ) {
          delta_q = tmp_delta;
  	  if( com1 < com2 ) { join1 = com1; join2 = com2; }
	  else { join1 = com2; join2 = com1; }
	  got_first = true;
        }
      }
    }
#endif
    delta_q = getNextJoin(net, node_community, e_ij, a_i, com1, com2);
    //Now we have the biggest delta_q, and com1 and com2 are set
    if( com1 < com2 ) { join1 = com1; join2 = com2; }
    else { join1 = com2; join2 = com1; }
   
    update(node_community, e_ij, a_i, com1, com2);
    //cerr << "join (" << join1 << ", " << join2 << ") = " << delta_q << endl;
    //cerr << "e_ij.size() == " << e_ij.size() << endl;
  
    //Record Q:
    joins[step] = pair<int,int>( join1, join2 );
    q_t[step + 1] = q_t[step] + delta_q;
    //cerr << "q[" << step << "]= " << q_t[step] << endl;
    step++;
    if( q_t[step] > q_max ) {
      q_max = q_t[step];
      step_max = step;
    }
  }
  return step_max;
}

NetworkPartition* AgglomPart::getCommunity(const Network& net, int step,
		    const std::vector< std::pair<int, int> >& joins)
{
  //Remake the node_community structure:
  int community = 0;
  vector<cnt_ptr<Network> > comm_node;
  comm_node.resize( net.getNodeSize() );
  auto_ptr<NodeIterator> ni( net.getNodeIterator() );
  while( ni->moveNext() ) {
    //Make networks with just one node
    comm_node[community] = net.newNetwork();
    comm_node[community++]->add( ni->current() );
  }
  /**
   * Recreate the step with the best structure:
   * Keep in mind, join[0] is the join associated
   * with q[1], since q[0] is the null join, or
   * every node in its very own community.
   * q[i] corresponds to join[i-1] (for i>0)
   * So, if q[step_max] is q_max, then
   * we want to join up to and including
   * join[step_max-1]
   */
  int join1, join2;
  for(int k = 0; k < step; k++) {
    join1 = joins[k].first;
    join2 = joins[k].second;
    comm_node[ join1 ]->add( comm_node[ join2 ].get() );
    comm_node[ join2 ]->clear();
  }
  //Prepare the output:
  vector< cnt_ptr<Network> >* out = new vector<cnt_ptr<Network> >();
  for(int k = 0; k < comm_node.size(); k++) {
    if( comm_node[k]->getNodeSize() > 0 ) {
      //Add the edges 
      comm_node[k]->addJoiningEdgesFrom( &net );
      //Copy the pointer to the output:
      out->push_back( comm_node[k] );
      comm_node[k] = 0;
    }
  }
  //Sort the output:
  sort(out->begin(), out->end(), ptr_gt<cnt_ptr<Network> >());
  return new NetworkPartition(net, out);
}

void AgglomPart::update(std::map<Node*, int>& community_map,
		        std::map<int, std::map<int, double> >& e_ij,
		        std::vector< double >& a_i, int join1, int join2)
{
  if( join1 != join2 ) {
    std::map<int, std::map<int, double> >::iterator mit;
    //update the number of edges between communities:
    for(mit = e_ij.begin(); mit != e_ij.end(); mit++) {
      int j = mit->first;
      if( j != join2 ) {
        e_ij[join1][j] += e_ij[join2][j];
        e_ij[j][join1] += e_ij[j][join2];
      }
    }
    a_i[join1] += a_i[join2];
    //Delete all of join2:
    e_ij[join1][join1] += e_ij[join2][join2];
    mit = e_ij.find(join2);
    e_ij.erase(mit);
    std::map<int, double>::iterator mit2;
    for(mit = e_ij.begin(); mit != e_ij.end(); mit++) {
      mit2 = mit->second.find(join2);
      mit->second.erase(mit2);
    }
    a_i[join2] = 0.0;
    //Put all the join2 nodes into join1
    map<Node*, int>::iterator it;
    for(it = community_map.begin();
        it != community_map.end();
        it++) {
      //Assign all nodes in community 2 to community 1
      if( it->second == join2 ) {
        it->second = join1;
      }
    }
  }
  else {
    //The algorithm told us to join a community with itself
  }
}
