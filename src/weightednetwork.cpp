#include "weightednetwork.h"

using namespace Starsky;
using namespace std;

WeightedNetwork::WeightedNetwork() : Network() { }

WeightedNetwork::WeightedNetwork(std::istream& in){
  readFrom(in);
}

WeightedNetwork::WeightedNetwork(const Network& net)
{
  Network::operator=(net);
}

#if 0
//This assumes weighted graphs have a certain file format
//which can only be used to store graphs with integer weight.
bool WeightedNetwork::add(Edge* edge) {

    if( edge_set.count(edge) == 0 ) {
      //This is a new edge:
     
      //Account for the edges and nodes
      pair< EdgeSet::iterator, bool> result;
      result = edge_set.insert(edge);
      if ( result.second == true ) {
	//This is a new edge, so bump the ref count:
	incrementEdgeRefCount(edge);
	//make sure these nodes are in the network
	Network::add(edge->first);
	Network::add(edge->second);
         
        connection_map[edge->first].insert(edge->second);
        connection_map[edge->second].insert(edge->first);

	_node_to_edges[edge->first].insert(edge);
	_node_to_edges[edge->second].insert(edge);
      }
      return result.second;
    }
    else {
	    /*Note that Joe has added the following lines
	     */
      EdgeSet::iterator e_it;
      e_it = edge_set.find(edge);
      if(e_it != edge_set.end() ){
	      double curr_weight = (*e_it)->getWeight();
	      (*e_it)->setWeight(curr_weight + 1.0);
      }
      //end of Joe's modification	      
      return false;
    }
}

#endif

int WeightedNetwork::getCommunities(vector<double>& q_t,
		            vector< pair<int,int> >& joins) const {
  //Which community does a given node belong to:
  map<Node*, int> node_community;

  //Initialize everything here:
  NodePSet::const_iterator n_it;
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
  EdgeSet::const_iterator e_it;
  int com1, com2;
  double e_total = 0.0;
  double weight;
  for(e_it = edge_set.begin();
      e_it != edge_set.end();
      e_it++) {
    com1 = node_community[ (*e_it)->first ];
    com2 = node_community[ (*e_it)->second ];
    weight = (*e_it)->getWeight();
    e_ij[com1][com2] += weight;
    e_ij[com2][com1] += weight;
    e_total += 2.0 * weight;
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
    //Iterate over each edge and find the biggest increase in Q
    got_first = false;
    //If we do no joins, there is delta_q = 0.0
    delta_q = 0.0;
    //These are not valid communities, but initialize to this:
    com1 = -1;
    com2 = -1;
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
    //Now we have the biggest delta_q
    
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
    for(n_it = node_set.begin();
        n_it != node_set.end();
        n_it++) {
      if( node_community[ *n_it ] == join2 ) {
        node_community[ *n_it ] = join1;
      }
    }  
  
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

map<double, int> WeightedNetwork::getEdgeWeightDist(const EdgeSet& edges) const {

  EdgeSet::const_iterator i = edges.begin();
  map<double, int> edgeweight_dist;
  double rep = 0;
  
  while(i != edges.end()) {
    rep = (*i)->getWeight();
    if( edgeweight_dist.count(rep) == 1 ) {
      edgeweight_dist[rep] = edgeweight_dist[rep] + 1;
    }
    else {
      edgeweight_dist[rep] = 1;
    }
    i++;
  }
  return edgeweight_dist;
}

map<double, int> WeightedNetwork::getEdgeWeightDist() const {
  return getEdgeWeightDist( edge_set );
}

void WeightedNetwork::printWeights(ostream& out) const{
    EdgeSet::const_iterator i;
    for(i = edge_set.begin(); i != edge_set.end(); i++){
	    out << (*i)->toString() << " with weight = "
		<< (*i)->getWeight() << endl;
    }
}

void WeightedNetwork::readFrom(istream& in) {
  SuperString line;
  map<string, Node*> name_map;
  while( getline(in, line, '\n') ) {
    if( line[0] == '#' ) { //This is a comment
      continue;
    }
    vector<SuperString> result = line.split(" : ");
    Node* first = 0;
    Node* second = 0;
    if( name_map.find(result[0]) == name_map.end() ) {
      //This is a new node:
      if( result[0] != "" ) {
        first = new Node(result[0]);
        name_map[ result[0] ] = first;
        add( first );
      }
    }
    else {
      first = name_map[ result[0] ];
    }
    if( result.size() == 1 ) { //There was no neighbors
      continue;
    }
    double weight = 1.0;
    if( result.size() > 2 ) {
      //The third colon holds the weight of these edges.
      weight = atof( result[2].c_str() );
    }
    result = result[1].split(" ");
    vector<SuperString>::iterator sit;
    for(sit = result.begin();
	sit != result.end();
	sit++) {
      second = 0;
      if( name_map.find( *sit ) == name_map.end() ) {
        //This is a new node:
	if( *sit != "" ) {
          second = new Node( *sit );
          name_map[ *sit ] = second;
	  add( second );
	}
      }
      else {
        second = name_map[ *sit ];
      }
      //Add the edge:
      if( first && second ) {
        add( Edge(first, second, weight) );
      }
    }
  }
}
