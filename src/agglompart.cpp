#include "newmancom.h"

using namespace std;
using namespace Starsky;

set<Network*>* AgglomPart::partition(const Network& input) {

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
  const Network::NodePSet& node_set = net.getNodes();

  //Initialize everything here:
  Network::NodePSet::const_iterator n_it;
  int community = 0;
  for(n_it = node_set.begin();
      n_it != node_set.end();
      n_it++) {
    node_community[ *n_it ] = community;
    community++;
  }
  //initialize e_ij:
  //This is the fraction of edges that go from
  //community i to j (it is symmetric).
  vector< vector<double> > e_ij;
  vector< vector<double> >::iterator it;
  e_ij.resize( node_set.size() );
  for( it = e_ij.begin();
       it != e_ij.end();
       it++) {
    it->resize( node_set.size() );
  }
  //Initialize e_ij
  for(int i = 0; i < e_ij.size(); i++) {
    for(int j = 0; j < e_ij[i].size(); j++) {
      e_ij[i][j] = 0.0;
    }
  }
  const Network::EdgeSet& edge_set = net.getEdges();
  Network::EdgeSet::const_iterator e_it;
  int com1, com2;
  double e_total = 0.0;
  for(e_it = edge_set.begin();
      e_it != edge_set.end();
      e_it++) {
    com1 = node_community[ (*e_it)->first ];
    com2 = node_community[ (*e_it)->second ];
    e_ij[com1][com2] += 1.0;
    e_ij[com2][com1] += 1.0;
    e_total += 2.0;
  }
  //Normalize e_ij:
  for(int i = 0; i < e_ij.size(); i++) {
    for(int j = 0; j < e_ij[i].size(); j++) {
      if( e_ij[i][j] > 0.0 ) {
        e_ij[i][j] /= e_total;
      }
    }
  }
  //Make a_i;
  vector<double> a_i;
  a_i.resize( e_ij.size() );
  for(int i = 0; i < a_i.size(); i++) {
    a_i[i] = 0.0;
    for(int j = 0; j < e_ij[i].size(); j++) {
      a_i[i] += e_ij[i][j];
    }
  }
  joins.clear();
  joins.resize( e_ij.size() - 1 );
  q_t.clear();
  q_t.resize( e_ij.size() );
  //We don't neccesarily start at Q=0;
  double q = 0.0;
  for(int i = 0; i < e_ij.size(); i++) {
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

set<Network*>* AgglomPart::getCommunity(const Network& net, int step,
		    const std::vector< std::pair<int, int> >& joins)
{
  const Network::NodePSet& node_set = net.getNodes();
  //Remake the node_community structure:
  int community = 0;
  vector<Network::NodePSet> comm_node;
  comm_node.resize( node_set.size() );
  Network::NodePSet::const_iterator n_it;
  for(n_it = node_set.begin();
      n_it != node_set.end();
      n_it++) {
    comm_node[community++].insert( *n_it );
  }
  //Recreate the step with the best structure:
  int join1, join2;
  for(int k = 0; k < step; k++) {
    join1 = joins[k].first;
    join2 = joins[k].second;
    comm_node[ join1 ].insert( comm_node[ join2 ].begin(),
		               comm_node[ join2 ].end() );
    comm_node[ join2 ].clear();
  }
  //Prepare the output:
  set< Network* >* out = new set<Network*>();
  set<Network*>& output = *out;
  for(int k = 0; k < comm_node.size(); k++) {
    if( comm_node[k].size() > 0 ) {
      output.insert( net.getSubNet( comm_node[k] ) );
    }
  }
  return out;
}

void AgglomPart::update(std::map<Node*, int>& community_map,
		        std::vector< std::vector<double> >& e_ij,
		        std::vector< double >& a_i, int join1, int join2)
{
  if( join1 != join2 ) {
    //update the number of edges between communities:
    for(int j = 0; j < e_ij.size(); j++) {
      if( j != join2 ) {
        e_ij[join1][j] += e_ij[join2][j];
        e_ij[j][join1] += e_ij[j][join2];
      }
    }
    a_i[join1] += a_i[join2];
    //Delete all of join2:
    e_ij[join1][join1] += e_ij[join2][join2];
    for(int j = 0; j < e_ij.size(); j++) {
      e_ij[join2][j] = 0.0;
      e_ij[j][join2] = 0.0;
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
