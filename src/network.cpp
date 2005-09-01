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

#include "network.h"

using namespace std;
using namespace Starsky;

//Initialization of the static empty node set
const Network::ConnectedNodePSet Network::_empty_cnodeset;
const Network::NodePSet Network::_empty_nodeset;
const Network::EdgeSet Network::_empty_edgeset;

map<Node*, int> Network::_node_ref_count;
map<Edge*, int> Network::_edge_ref_count;

Network::Network()  {

}

Network::Network(const Network& net) {
  operator=(net);
}

Network::~Network() {
    clear();
}

Network::Network(istream& input) {
  readFrom(input);
}

bool Network::add(const Edge& e) {
   Edge* n_e = getEdgePtr(e);
   if( e.first == e.second ) {
	   cerr << "adding a loop: (" << e.first << ", " << e.second
		<< ")"  << endl;
   }
   if( n_e == 0 ) {
     n_e = new Edge();
     *n_e = e;
   }
   return add(n_e);
}

bool Network::add(Edge* edge) {
    
    bool success = false;
    if( edge_set.count(edge) == 0 ) {
      //This is a new edge:
      list<INetworkMonitor*>::iterator nm_it;
      for(nm_it = _net_mon.begin(); nm_it != _net_mon.end(); nm_it++) {
        (*nm_it)->preEdgeAdd(edge);
      }
     
      //Account for the edges and nodes
      pair< EdgeSet::iterator, bool> result;
      result = edge_set.insert(edge);
      if ( result.second == true ) {
	//This is a new edge, so bump the ref count:
	incrementEdgeRefCount(edge);
	//make sure these nodes are in the network
	add(edge->first);
	add(edge->second);
         
        connection_map[edge->first].insert(edge->second);
        connection_map[edge->second].insert(edge->first);

	_node_to_edges[edge->first].insert(edge);
	_node_to_edges[edge->second].insert(edge);
      }
      success = result.second;
      //Tell the network monitors
      for(nm_it = _net_mon.begin(); nm_it != _net_mon.end(); nm_it++) {
        (*nm_it)->postEdgeAdd(edge);
      }
    }
    else {
      success = false;
    }
    return success;
}

bool Network::add(Node* node) {
  if( node_set.find(node) == node_set.end() ) {
    list<INetworkMonitor*>::iterator nm_it;
    for(nm_it = _net_mon.begin(); nm_it != _net_mon.end(); nm_it++) {
      (*nm_it)->preNodeAdd(node);
    }
    pair<NodePSet::iterator, bool> result;
    result = node_set.insert(node);
    bool success = result.second;
    if(success) {
        incrementNodeRefCount(node);
    }
    for(nm_it = _net_mon.begin(); nm_it != _net_mon.end(); nm_it++) {
      (*nm_it)->postNodeAdd(node);
    }
    return result.second;
  }
  else {
    //The node was already there:
  }
  return false;
}

void Network::add(INetworkMonitor* nm)
{
  _net_mon.push_back(nm);
  nm->postAdd(this);
}

void Network::clear() {
    //Delete all the memory
    NodePSet::iterator i;
    for(i=node_set.begin(); i != node_set.end(); i++ ) {
      decrementNodeRefCount( *i );
    }
    node_set.clear();

    EdgeSet::iterator j;
    for(j=edge_set.begin(); j != edge_set.end(); j++) {
      decrementEdgeRefCount( *j );
    }
    connection_map.clear();
    edge_set.clear();
    _node_to_edges.clear();
}

int Network::decrementEdgeRefCount(Edge* edge) {
    int ret = -1;
    map<Edge*, int>::iterator ref_it = _edge_ref_count.find(edge);
    if( ref_it != _edge_ref_count.end() ) {
      ref_it->second = ref_it->second - 1;
      ret = ref_it->second;
      if( ref_it->second == 0 ) {
        delete edge;
	_edge_ref_count.erase( ref_it );
      }
    }
    else {
      cerr << "tried to decrement edge: " << edge << " past 0" << endl;
    }
    return -1;
}

int Network::decrementNodeRefCount(Node* node) { 
    int ret = -1;
    map<Node*, int>::iterator ref_it = _node_ref_count.find(node);
    if( ref_it != _node_ref_count.end() ) {
        ref_it->second = ref_it->second - 1;
        ret = ref_it->second;
	if( ref_it->second == 0) {
          delete node;
	  _node_ref_count.erase( ref_it );
	}
    }
    else {
      cerr << "tried to decrement node: " << node << " past 0" << endl;
    }
    return ret;
    
}

double Network::getAssortativity() const {

  /*
   * We need to use doubles because we can easily have sums
   * which exceed the maximum size of an integer.
   */
	
  //int j,k,sum_j, sum_k, sum_jk, sum_j2, sum_k2;
  double j,k,sum_j, sum_k, sum_jk, sum_j2, sum_k2;
  sum_j = 0;
  sum_k = 0;
  sum_jk = 0;
  sum_j2 = 0;
  sum_k2 = 0;
  EdgeSet::const_iterator i;
  //Add the neighbors:
  for(i = edge_set.begin(); i != edge_set.end(); i++) { 
    //We need "remaining degree" for this calculation
    j = (double)(getDegree( (*i)->first ) - 1);
    k = (double)(getDegree( (*i)->second ) - 1);
    //DEBUG:
    //cerr << "j: " << j << " k: " << k << endl;
    sum_j += j;
    sum_k += k;
    sum_jk += j * k;
    sum_j2 += j * j;
    sum_k2 += k * k;
  }
  double m_inv = 1.0 / (double)(edge_set.size());
  double r, t;

  /*
   * r = \frac{M^{-1}(\sum_i j_i k_i) - \left(M^{-1}\sum_i \frac{j_i + k_i}{2}\right)^{2}}
   *          {M^{-1}\sum_i\frac{j_i^2 k_i^2}{2} - \left(M^{-1}\sum_i \frac{j_i + k_i}{2}\right)^2}
   */
  t = m_inv * 0.5 * (double)(sum_j + sum_k);
  r = (m_inv * (double)(sum_jk) - t*t) / (m_inv * 0.5 * (double)(sum_j2 + sum_k2) - t*t);
  /*
  //DEBUG:
  cerr << "sum_j: " << sum_j << endl
       << "sum_k: " << sum_k << endl
       << "sum_jk: " << sum_jk << endl
       << "sum_j2: " << sum_j2 << endl
       << "sum_k2: " << sum_k2 << endl
       << "m_inv: " << m_inv << endl
       << "edge_set.size(): " << edge_set.size() << endl
       << "t: " << t << endl
       << "r: " << r << endl;
       */
  return r;
}

int Network::getAssociatedNumber(Node* aNode) const {

    /**
     * \todogetDistance never returns -1, maybe we want to do so
     * in networks with more than one component
     */
    return getDistance(aNode);
}

map<int, int> Network::getAssociatedNumberDist() const {
    NodePSet::const_iterator n_it;
    int this_an;
    map<int, int> ret_val;
    
    for( n_it = node_set.begin(); n_it != node_set.end(); n_it++) {
        this_an = getAssociatedNumber( *n_it );
	ret_val[this_an] = ret_val[this_an] + 1;
    }
    return ret_val;
}

double Network::getAverageDegree(const NodePSet& nodes) const {

  return getDegreeMoment(1,nodes);
}

double Network::getAverageDegree() const {
    return getAverageDegree( node_set );
}

vector<int> Network::getCCHist(int bins) const {

    vector<int> ret_val;
    double this_cc;
    int this_bin;
    NodePSet::const_iterator n_it;
    
    ret_val.resize(bins);
    for(int j = 0; j < bins; j++) {
        ret_val[j] = 0;
    }
    
    for(n_it = node_set.begin(); n_it != node_set.end(); n_it++) {
      if( getDegree( *n_it ) != 0 ) {
        this_cc = getClusterCoefficient( *n_it );
	this_bin = (int)(this_cc * (double)bins);
	if( this_cc < 1.0) {
	  ret_val[this_bin] = ret_val[this_bin] + 1;
	}
	else {
          ret_val[bins - 1] = ret_val[bins - 1] + 1;
	}
      }
    }
    return ret_val;

}

map<int, double> Network::getCCvsDegree() const {

    NodePSet::const_iterator n_it;
    map<int, set<double> > all_cc;
    map<int, set<double> >::const_iterator all_it;
    set<double>::const_iterator d_it;
    map<int, double> ret_val;
    int degree;
    double cc;
    
    for(n_it = node_set.begin(); n_it != node_set.end(); n_it++) {
        cc = getClusterCoefficient( *n_it );
	degree = getDegree( *n_it );
	all_cc[degree].insert(cc);
    }
    double average;
    for(all_it = all_cc.begin(); all_it != all_cc.end(); all_it++) {
	average = 0.0;
        for(d_it = all_it->second.begin();
	    d_it != all_it->second.end();
	    d_it++) {

            average += *d_it;
	}
	average /= (double)(all_it->second.size());
	ret_val[all_it->first] = average;
    }
    return ret_val;
	
}

double Network::getCCStdErr() const {
  double cc = getClusterCoefficient();
  if( cc == 0.0 ) {
    /*
     * If the whole network has cc = 0, removing nodes won't change it
     */
    return 0.0;
  }
  NodePSet::const_iterator n_it;
  double sigma2 = 0.0;
  for(n_it = getNodes().begin(); n_it != getNodes().end(); n_it++) {
    Network tmp = *this;
    tmp.remove( *n_it );
    double tmp_cc = tmp.getClusterCoefficient();
    sigma2 += (cc - tmp_cc) * (cc - tmp_cc);
  }
  double n_count = (double)getNodes().size();
  return sqrt( (n_count - 1.0) * sigma2/n_count );
}

const Network::EdgeSet& Network::getMinCut(Node* node1,Node* node2) const {
  ///\todo
}

#if 0
int Network::getCommunities(vector<double>& q_t,
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

set<Network> Network::getCommunity(int step,
			            const vector< pair<int, int> >& joins)
                                    const
{
  //Remake the node_community structure:
  int community = 0;
  vector<NodePSet> comm_node;
  comm_node.resize( node_set.size() );
  NodePSet::const_iterator n_it;
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
  set< Network > output;
  for(int k = 0; k < comm_node.size(); k++) {
    if( comm_node[k].size() > 0 ) {
      output.insert( getSubNet( comm_node[k] ) );
    }
  }
  return output;
}


set<Network> Network::getComponents() const {

    set<Network> output;
    
    Network* tmp_net = 0;
    
    NodePSet to_check, checked;
    NodePSet::const_iterator i;
    NodePSet::iterator check_it;
    
    ConnectedNodePSet::const_iterator n_it;
    
    EdgeSet tmp_edges;
    EdgeSet::const_iterator e_it;
    
    //For all nodes
    for(i = node_set.begin(); i != node_set.end(); i++) {
	//Check to make sure we have not already identified this component:
	if( checked.find( *i ) == checked.end() ) {
            to_check.insert( *i );
            check_it = to_check.begin();
	    //Here we make a new component:
	    tmp_net = new Network();
            while( check_it != to_check.end() ) {
		tmp_net->add( *check_it );
                tmp_edges = getEdges( *check_it );
	        for(e_it = tmp_edges.begin(); e_it != tmp_edges.end(); e_it++) {
                    tmp_net->add( *e_it );
	        }
	        for(n_it = getNeighbors( *check_it ).begin();
		    n_it != getNeighbors( *check_it ).end();
		    n_it++) {
		    if( checked.find( *n_it ) == checked.end() ) {
                      to_check.insert( *n_it );
		    }
		    //Else we have already checked this one.
	        }
	        checked.insert( *check_it );
                to_check.erase( check_it );
	        check_it = to_check.begin();
            }
	    checked.insert( *i );
	    //We have reached the entire component, go to the next.
	    output.insert( *tmp_net );
	    delete tmp_net;
	    tmp_net = 0;
        }
	else {
	}
    }
    return output;
}
#endif

double Network::getDegreeMoment(int m) const
{
  return getDegreeMoment(m,node_set);
}

double Network::getDegreeMoment(int m, const NodePSet& nodes) const
{
    double ave = 0.0;
    NodePSet::const_iterator i;
    if( m == 1) {
      for(i = nodes.begin(); i != nodes.end(); i++) {
          ave += (double)getDegree( *i );
      }
    }
    else if( m == 2) {
      for(i = nodes.begin(); i != nodes.end(); i++) {
          ave += (double)(getDegree( *i ) * getDegree( *i ));
      }	
    }
    else {
      for(i = nodes.begin(); i != nodes.end(); i++) {
          ave += pow( (double)getDegree( *i ), m);
      }
    }
    return ave / (double)( nodes.size() );
}


const Network::ConnectedNodePSet& Network::getNeighbors(Node* node) const {
  map< Node*, ConnectedNodePSet >::const_iterator i = connection_map.find(node);
  if( i != connection_map.end() ) {
    return i->second;
  }
  else {
    //returns an empty set
    return _empty_cnodeset;
  }
}

double Network::getClusterCoefficient(Node* node) const {
  int degree = getDegree(node);
  if( degree > 1 ) {
    int triangles = getTriangles(node);
    double dd = (double)degree;
    double d2 = dd * (dd - 1.0)/2.0;
    return (double)triangles/d2;
  }
  else {
    //This is not well defined;
    return -1.0;
  }
}
#if 0
double Network::getClusterCoefficient(Node* node) const {
    map<Node*, ConnectedNodePSet >::const_iterator i, j;
    ConnectedNodePSet::const_iterator k,l;

    /*
     * edges counts the number of edges between the nodes that node is connected
     * to.  The clustering coefficient is that number, divided by the maximum.
     */
    
    int edges = 0;
    int i_deg = 0;
   
    i = connection_map.find(node);
    if( i != connection_map.end() ) {
      i_deg = i->second.size();
      if ( i_deg == 0 ) {
	  //Maybe we should return -1.0 here.
	  //Clustering does not make sense for isolated nodes
          return -1.0;
      }
      if ( i_deg == 1 ) {
	  //If you have degree 1, return -1.0.
          return -1.0;
      }
      for(k = i->second.begin(); k != i->second.end(); k++) {
	l = k;
	l++;
	for(; l != i->second.end(); l++) {
          j = connection_map.find(*k);
          if( j != connection_map.end() ) {
            edges += j->second.count(*l);
 	  }
	}
      }
      return (double)(edges)/(double)((i_deg * (i_deg - 1))/2);
    }
    else {
      //This is an error value
      return -1.0;
    }
}
#endif

double Network::getClusterCoefficient(const NodePSet& nodes) const {
    double out = 0.0;
    double this_cc = 0.0;
    int nodes_count = 0;
    NodePSet::const_iterator i;
    for(i = nodes.begin(); i != nodes.end(); i++) {
	//Skip nodes that have 1 degrees or less
        if( getDegree( *i ) > 1 ) {
          this_cc = getClusterCoefficient(*i);
	  ++nodes_count;
          out += this_cc;
	}
    }
    if ( nodes_count == 0 ) { return 0.0; }
    return out / (double)( nodes_count );
}

double Network::getClusterCoefficient() const {
    return getClusterCoefficient(node_set);
}

int Network::getDegree(Node* node) const {
    map<Node*, EdgeSet >::const_iterator i = _node_to_edges.find(node);
    
    if( i != _node_to_edges.end() ) {
      return i->second.size();
    }
    else {
      return 0;
    }
}

map<int, int> Network::getDegreeDist(const NodePSet& nodes) const {

  NodePSet::const_iterator i = nodes.begin();
  map<int, int> deg_dist;
  int deg = 0;
  
  while(i != nodes.end()) {
    deg = getDegree( *i );
    if( deg_dist.count(deg) == 1 ) {
      deg_dist[deg] = deg_dist[deg] + 1;
    }
    else {
      deg_dist[deg] = 1;
    }
    i++;
  }
  return deg_dist;
}

map<int, int> Network::getDegreeDist() const {
  return getDegreeDist( node_set );
}

double Network::getDegreeEntropy() const {
	
  map<int, int> deg_dist = getDegreeDist();
  map<int, int>::const_iterator deg_it;
  double entropy = 0;
  for(deg_it = deg_dist.begin(); deg_it != deg_dist.end(); deg_it++) {
    entropy -= deg_it->second * log( (double)deg_it->second );
  }
  entropy /= node_set.size();
  entropy += log( (double)node_set.size() );
  return entropy;
}

int Network::getDistancesFrom(Node* start,
		              std::map<Node*, int>& distances,
			      std::map<Node*, int>& weights,
			      std::set<Node*>& leaf_nodes,
			      int max_depth) const {
  
  //make sure the output is intialized
  distances.clear();
  weights.clear();
  leaf_nodes.clear();
  //This is a queue to keep track of the nodes doing a breadth first search
  queue<Node*> to_visit;
  distances[start] = 0;
  weights[start] = 1;
  int max_dist = 0,
      distance = 0,
      neighbor_max_dist = 0,
      weight = 1;
  to_visit.push(start);
  Node* tmp_node = 0;
  NodePSet::const_iterator n_it;
  //Only go a certain depth:
  bool is_max_depth = (max_depth != -1);
  map<Node*, int>::iterator dit;
  
  while( to_visit.size() > 0 ) {
    tmp_node = to_visit.front();
    distance = distances[tmp_node] + 1;
    weight = weights[tmp_node];
    //Just stop if the distance is more than we should go
    if( is_max_depth && (distance > max_depth) ) { break; }
    neighbor_max_dist = 0;
    for(n_it = getNeighbors(tmp_node).begin();
	n_it != getNeighbors(tmp_node).end();
	n_it++) {
      dit = distances.find( *n_it );
      if( dit == distances.end() ) {
	//We have not yet visited this node
	distances[*n_it] = distance;
	weights[*n_it] = weight;
	to_visit.push( *n_it );
	//update the maximum
	max_dist = (max_dist < distance ) ? distance : max_dist;
      }
      else if( dit->second == distance ) {
        //There is more than one way to reach this node:
	weights[*n_it] += weight;
      }
      if( neighbor_max_dist < distances[*n_it] ) {
        neighbor_max_dist = distances[*n_it];
      }
    }
    /**
     * We check all the nodes at distance d before d+1 (breadth first)
     * so, once a distance is assigned it is never decreased
     * Hence, we know now the distance of all tmp_node neighbors.
     * If they are all less than or equal to distance from tmp_node,
     * then tmp_node must be a leaf node
     */
    if( neighbor_max_dist <= distances[tmp_node] ) {
      leaf_nodes.insert(tmp_node);
    }
    //Remove the first element in the list
    to_visit.pop();
  }
  return max_dist; 
}

int Network::getDistance(Node* start, Node* end) const {

    //A slight optimization, we could also get all the neighbors and check:
    if (start == end) return 0;
    map<Node*, int> distances, weights;
    map<Node*, int>::iterator dit;
    set<Node*> leafs;
    //We just find all the distances:
    int max = getDistancesFrom(start, distances, weights, leafs);
    if( end != 0) {
      dit = distances.find(end);
      if( dit != distances.end() ) { return dit->second; }
    }
    if( end == 0 ) { return max; }
    //We only get here if there is no path from start to end
    return -1;
}

vector<int> Network::getDistanceDist(Node* node, int max) const {

   
    map<Node*, int> distances, weights;
    set<Node*> leafs;
    int max_dist = getDistancesFrom(node, distances, weights, leafs, max);
    //Now we have the distance from node to all others:
    //Get the greatest distance:
    vector<int> ret_val;
    ret_val.resize( max_dist + 1 );
      
    map<Node*, int>::iterator dit;
    for(dit = distances.begin(); dit != distances.end(); dit++) {
	ret_val[ dit->second ]++;
    }
    return ret_val;	
}

int Network::getDistanceDist(Node* start,
		             map<int, int>& distribution) const {
  map<Node*, int> distances, weights;
  map<Node*, int>::iterator dit;
  set<Node*> leaf_nodes;
  NodePSet::const_iterator n_it;
  
  int max = getDistancesFrom(start, distances, weights, leaf_nodes);
  for( n_it = getNodes().begin();
       n_it != getNodes().end();
       n_it++) {
    dit = distances.find( *n_it );
    if( dit == distances.end() ) {
      //This node is unreachable from start
      distribution[-1]++;
    }
    else {
      distribution[ dit->second ]++;
    }
  }
  return max;
}


int Network::getDistanceDist(map<int, int>& distribution) const {
  map<Node*, int> distances, weights;
  map<Node*, int>::iterator dit;
  set<Node*> leaf_nodes;
  NodePSet::const_iterator n_it;
  NodePSet::const_iterator j_it;
  
  int max = -1,
      tmp_max,
      radius = 0;
  for(j_it = getNodes().begin(); j_it != getNodes().end(); j_it++) {
    tmp_max = getDistancesFrom(*j_it, distances, weights, leaf_nodes);
    max = (max < tmp_max) ? tmp_max : max;
    radius = (tmp_max < radius) ? tmp_max : radius;
    for( n_it = getNodes().begin();
         n_it != getNodes().end();
         n_it++) {
      dit = distances.find( *n_it );
      if( dit == distances.end() ) {
        //This node is unreachable from start
        distribution[-1]++;
      }
      else {
        distribution[ dit->second ]++;
      }
    }
  }
  //It is easy to find the center if we want to do that...
  return radius;
}

map<int, int> Network::getDistanceDist(const NodePSet& nodes) const {
    map<int, int> ret_val;
    NodePSet::const_iterator n_it;
    for( n_it = nodes.begin();
	 n_it != nodes.end();
	 n_it++) {
      getDistanceDist(*n_it, ret_val);
    }
    return ret_val;
}

map<int, int> Network::getDistanceDist() const {
    //Just call the above on all nodes
    map<int, int> ret_val;
    getDistanceDist( ret_val );
    return ret_val;
}

Edge* Network::getEdgeBetweennessFor(Node* target,
		                     map<Edge*, double>& my_between) const
{
  //Start a new betweeness calculation
  my_between.clear();
  map<Node*, int> distances, weights;
  set<Node*> leafs;
  //Get the weights of all the nodes and find the leaf nodes
  int mdist = getDistancesFrom(target, distances, weights, leafs);
  //Initialize the edges from the leafs:
  set<Node*>::iterator leaf_it;
  EdgeSet::iterator e_it;
  Node* tmp_node, *other;
  map<Node*, EdgeSet>::const_iterator es_it;
  double max_between = 0.0, tmp_between = 0.0;
  Edge* max_edge = 0;
  /**
   * Here is map so we can go from greatest to shortest distance.
   * This ensures that we have always calculated the "below score"
   * before trying to compute score for an edge
   */
  map<int, NodePSet> distance_to_nodes;
  for( leaf_it = leafs.begin();
       leaf_it != leafs.end();
       leaf_it++) {
    es_it = _node_to_edges.find(*leaf_it);
    for(e_it = es_it->second.begin();
	e_it != es_it->second.end();
	e_it++) {
      other = (*e_it)->getOtherNode( *leaf_it );
      if( leafs.find( other ) == leafs.end() ) {
        //This edge does not go to another leaf:
	tmp_between = (double)weights[other] /
		                    (double)weights[*leaf_it];
	my_between[ *e_it ] = tmp_between;
        if( tmp_between > max_between ) {
          max_between = tmp_between;
	  max_edge = *e_it;
	}
	//Climb it in an order.  When we insert we see if
	//it is new or not:
	distance_to_nodes[ distances[other] ].insert(other);
      }
    }
  }
  map<Edge*, double>::iterator betit;
  bool have_all_below = false;
  EdgeSet above_edges;
  //We have initialized the first edges
  map<int, NodePSet>::reverse_iterator dit;
  NodePSet::iterator nit;
  for( dit = distance_to_nodes.rbegin();
       dit != distance_to_nodes.rend();
       dit++) {
   while( dit->second.size() > 0 ) {
    nit = dit->second.begin();
    //Take care of the next node
    tmp_node = *nit;
    //Initialize variables for the below score
    double below_score = 0.0;
    above_edges.clear();
    have_all_below = true;
    //Look at all the edges for this node to get the below score
    es_it = _node_to_edges.find(tmp_node);
    for(e_it = es_it->second.begin();
	e_it != es_it->second.end();
	e_it++) {
      other = (*e_it)->getOtherNode( tmp_node );
      if( distances[other] > distances[tmp_node] ) {
        //The neighbor is further from target
	betit = my_between.find(*e_it);
	if( betit != my_between.end() ) {
	  //We already have the score for this edge
	  below_score += betit->second;
	}
	else if( have_all_below ) {
	  //We have not scored this edge
	    have_all_below = false;
	    cerr << "Don't have all below!! Node: "
		 << tmp_node << " = " << tmp_node->toString() << endl;
	}
      }
      else if( distances[other] < distances[tmp_node] ) {
        //This is an above edge:
	above_edges.insert( *e_it );
      }
    }
    if( have_all_below ) {
      //We can compute the betweenness for all above_edges
      double weight_ratio = 0.0;
      for(e_it = above_edges.begin();
	  e_it != above_edges.end();
	  e_it++) {
        //Make sure we don't try to score edges twice
        if( my_between.find( *e_it ) == my_between.end() ) {
          other = (*e_it)->getOtherNode( tmp_node );
          weight_ratio = (double)weights[other]/(double)weights[tmp_node];
	  tmp_between = (below_score + 1.0) * weight_ratio;
	  my_between[ *e_it ] = tmp_between;
          if( tmp_between > max_between ) {
            max_between = tmp_between;
	    max_edge = *e_it;
	  }
	  distance_to_nodes[ distances[other] ].insert( other );
        }
	else {
          //This should not happen:
	  cerr << "Tried to score an edge twice: "
	       << "Edge: " << *e_it << ", " << (*e_it)->toString() << endl;
	}
      }
      /**
       * We have computed the betweenness for all above edges
       * and since we go from large distance to small distance
       * we can never see this node again (all future nodes will
       * be as close or closer than this one, never further).
       */
      dit->second.erase(nit);
    }
   }
  }
  return max_edge;
}

Edge* Network::getEdgeBetweenness(map<Edge*, double>& betweenness) const {
  
  NodePSet::const_iterator n_it;
  Edge* max_edge = 0;
  double max_between = 0.0, tmp_between = 0.0;
  map<Edge*, double> my_between;
  for(n_it = getNodes().begin();
      n_it != getNodes().end();
      n_it++) {
    max_edge = getEdgeBetweennessFor(*n_it, my_between);
    /*
    cout << "source: " << (*n_it)->toString() << endl
	 << "max edge: " << max_edge->toString() << endl
	 << "betweenness: " << my_between[max_edge] << endl;
    */
    //We have score each of the appropriate edges:
    map<Edge*, double>::iterator betit;
    for(betit = my_between.begin();
        betit != my_between.end();
        betit++) {
      betweenness[ betit->first ] += betit->second;
      tmp_between = betweenness[ betit->first ];
      if( tmp_between > max_between ) {
        max_between = tmp_between;
        max_edge = betit->first;
      }
    }
  }
  return max_edge;
}

const Network::EdgeSet& Network::getEdges() const {
    return edge_set;
}

const Network::EdgeSet& Network::getEdges(Node* node) const {
    
    if(node == 0) {
        return edge_set;
    }
    map<Node*, EdgeSet >::const_iterator i;    
    i = _node_to_edges.find(node);
    if( i != _node_to_edges.end() ) {
	return i->second;
    }
    return _empty_edgeset;
}

Edge* Network::getEdge(Node* from, Node* to) const {
  if( (0 == from) || (0 == to) ) {
    return 0;
  }
  int df = getDegree(from);
  int dt = getDegree(to);
  if( (df == 0) || (dt == 0) ) {
    //One of them has no edges:
    return 0;
  }
  //Get the edges of the node with the smaller degree
  Node* check = from;
  if( df > dt ) {
    check = to;
  }
  map<Node*, EdgeSet>::const_iterator it;
  it = _node_to_edges.find(check);
  const EdgeSet& eds = it->second;
  EdgeSet::const_iterator eit = eds.begin();
  for(eit = eds.begin(); eit != eds.end(); eit++)
  {
    if( (*eit)->connects( from,  to ) ) {
      return *eit;
    }
  }
  return 0; 
}

map< pair<int,int>, int > Network::getEdgeDist() const {
  map<pair<int,int>, int > result;
  EdgeSet::const_iterator e_it;
  int d1, d2;
  for(e_it = edge_set.begin(); e_it != edge_set.end(); e_it++) {
    if( *e_it ) { 
     d1 = getDegree( (*e_it)->first );
     d2 = getDegree( (*e_it)->second );
     if( result.count( pair<int,int>(d1,d2) ) == 1) {
      result[ pair<int,int>(d1,d2) ] += 1;
     }
     else {
      result[ pair<int,int>(d1,d2) ] = 1;
     }
     if( result.count( pair<int,int>(d2,d1) ) == 1) {
      result[ pair<int,int>(d2,d1) ] += 1;
     }
     else {
      result[ pair<int,int>(d2,d1) ] = 1;
     }
   }
  }
  return result;
}

pair<double,double> Network::getEdgeEntropy() const {

  map<pair<int, int>,int > eij_count = getEdgeDist();
  map<pair<int,int>, int >::const_iterator it1;
  
  //Get the e_k probability distribution:
  map<int, int> edge_degs;
  for(it1 = eij_count.begin(); it1 != eij_count.end(); it1++) {
    edge_degs[it1->first.first] += it1->second;
  }
  
  map<int, int>::const_iterator it2;
  //Here is H(e_k):
  double h_ek = 0.0;
  for( it2 = edge_degs.begin(); it2 != edge_degs.end(); it2++) {
    if( it2->second > 0) {
      h_ek -= it2->second * log( (double)it2->second );
    }
  }
  h_ek /= (2 * edge_set.size());
  h_ek += log( (double)2 * edge_set.size() );
 
  //Here is H(e_ij):
  double h_eij = 0.0;
  for(it1 = eij_count.begin(); it1 != eij_count.end(); it1++) {
      if( it1->second > 0 ) {
        h_eij -= it1->second * log( (double) it1->second );
      }
  }
  h_eij /= (2 * edge_set.size());
  h_eij += log( (double)2 * edge_set.size() );

  return pair<double,double>(h_ek, h_eij);
}

double Network::getEdgeMutualInfo() const {
  pair<double, double> entropies = getEdgeEntropy();
  double h_i = entropies.first;
  double h_ij = entropies.second;
  return ( 2 * h_i - h_ij );
}

Edge* Network::getEdgePtr(const Edge& edge) const {
    
    //See that these guys actually connect to each other:
    map<Node*, ConnectedNodePSet>::const_iterator it;
    it = connection_map.find(edge.first);
    if( it == connection_map.end() || it->second.count( edge.second ) == 0 ) {
      return 0;
    }
    it = connection_map.find(edge.second);
    if( it == connection_map.end() || it->second.count( edge.first ) == 0) {
      return 0;
    }
    //Find the ptr of the matching edge:
    map<Node*, EdgeSet>::const_iterator it1, it2;
    it1 = _node_to_edges.find(edge.first);
    it2 = _node_to_edges.find(edge.second);
    //Make sure these nodes have some edges:
    if( (it1 == _node_to_edges.end()) || (it2 == _node_to_edges.end() ) ) {
      return 0;
    }
    
    EdgeSet::const_iterator eit1;
    //Look through the smaller set:
    if( it1->second.size() < it2->second.size() ) {
       for( eit1 = it1->second.begin(); eit1 != it1->second.end(); eit1++) {
         if( (*(*eit1))==edge ) {
           return *eit1;
	 }
       }
    }
    else {
        for( eit1 = it2->second.begin(); eit1 != it2->second.end(); eit1++) {
          if( (*(*eit1))== edge ) {
            return *eit1;
	  }
	}
    }
    return 0;
}

double Network::getExpectedTransitivity() const {
  
  double k2, k, exp_cc;
  int n = getNodes().size();
  double edges = (double)getEdges().size();
  map<int, int> degdist = getDegreeDist();
  map<int, int> degdist2 = degdist;
  map<int, int>::iterator degit,degit2;
  exp_cc = 0.0;
  double coeff;
  double deg,deg2,dcount,dcount2;
  k = getDegreeMoment(1);
  for(degit2=degdist2.begin(); degit2!=degdist2.end(); degit2++) {
    for(degit=degdist.begin(); degit!=degdist.end(); degit++) {
      deg = (double)degit->first - 1.0; //Excess degree
      dcount = (double)degit->second/((double)n) * (deg + 1.0)/k;
      deg2 = (double)degit2->first - 1.0; //Excess degree
      dcount2 = (double)degit2->second/((double)n) * (deg2 + 1.0)/k;
      coeff = (1.0 - deg*deg2/(2*edges*edges));
      coeff = pow(coeff,edges);
      exp_cc += (dcount * dcount2 * ( 1.0 - coeff));
    }
  }
  return exp_cc;
}


Network::NodePSet Network::getNeighborhood(Node* node, int distance) const {

 map<Node*, int> distances, weights;
 set<Node*> leaves;
 /* Use the general breadth first code */
 getDistancesFrom(node, distances, weights, leaves, distance);
 /* Now the nodes we reached are all the ones with a distance: */
 NodePSet result;
 map<Node*, int>::iterator n_it;
 for(n_it = distances.begin(); n_it != distances.end(); n_it++) {
   result.insert( n_it->first ); 
 }
 return result;
}

const Network::NodePSet& Network::getNodes() const {
    return node_set;
}

map<int, int> Network::getNthNeighborDist(const NodePSet& nodes, int nth) const {

  vector<int> distances;
  NodePSet::const_iterator n_it;
  map<int, int> ret;
  int count;
  
  for(n_it = nodes.begin(); n_it != nodes.end(); n_it++) {
    distances = getDistanceDist(*n_it,nth);
    if( (unsigned)nth < distances.size()) {
      count = distances[nth];
    }
    else {
      count = 0;
    }
    ret[count] = ret[count] + 1;
  }
  return ret;
}

map<int, int> Network::getNthNeighborDist(int nth) const {
  return getNthNeighborDist(node_set, nth);
}

Network* Network::getSubNet(const NodePSet& nodes) const {
  //Return a Network with just the nodes given
  NodePSet::const_iterator n_it;
  map<Node*, EdgeSet>::const_iterator map_it;
  EdgeSet::const_iterator e_it;
  Network* out = new Network();
  Network& output = *out;
  for( n_it = nodes.begin();
       n_it != nodes.end();
       n_it++) {
    output.add( *n_it );
    map_it = _node_to_edges.find( *n_it );
    if( map_it != _node_to_edges.end() ) {
      //Add the neighbors:
      for( e_it = map_it->second.begin();
	 e_it != map_it->second.end();
	 e_it++) {
        Node* other = (*e_it)->getOtherNode( *n_it);
        if( nodes.find( other ) != nodes.end() ) {
	  //Add the edge (using the same memory)
          output.add( *e_it );
        }
      }
    }
  }
  return out;
}

double Network::getTransitivity() const {
  int triangles, wedges;
  getTrianglesWedges(triangles,wedges);
  return 3.0 * (double)triangles/(double)wedges;
}

int Network::getTriangles(Node* node) const {
    map<Node*, ConnectedNodePSet >::const_iterator i, j;
    ConnectedNodePSet::const_iterator k,l;

    /*
     * edges counts the number of edges between the nodes that node is connected
     * to.  The clustering coefficient is that number, divided by the maximum.
     */
    
    int edges = 0;
    int i_deg = 0;
   
    i = connection_map.find(node);
    if( i != connection_map.end() ) {
      i_deg = i->second.size();
      if ( i_deg == 0 ) {
          return 0;
      }
      if ( i_deg == 1 ) {
          return 0;
      }
      for(k = i->second.begin(); k != i->second.end(); k++) {
	l = k;
	l++;
	for(; l != i->second.end(); l++) {
          j = connection_map.find(*k);
          if( j != connection_map.end() ) {
            edges += j->second.count(*l);
 	  }
	}
      }
      return edges; 
    }
    else {
      //Maybe we should throw an exception
      return 0;
    }
}

int Network::getTriangles(Edge* this_edge) const {

  /*
   * 
   */
  EdgeSet::const_iterator e2_it;
  map<Node*, EdgeSet>::const_iterator emap_it;
  int triangles = 0;
  int wedges = 0;
    //Look at the wedges from one end:
    Node* this_node = this_edge->first;
    Node* other1 = this_edge->second;
    emap_it = _node_to_edges.find(this_node);
    for( e2_it = emap_it->second.begin();
	 e2_it != emap_it->second.end();
	 e2_it++) {
      if( *e2_it != this_edge ) {
        //There is one more wedge:
        wedges++;
        //See if it is a triangle:
	Node* other = (*e2_it)->getOtherNode(this_node);
	map<Node*, ConnectedNodePSet>::const_iterator map_it;
	map_it = connection_map.find(other);
        if( map_it->second.find(other1) != map_it->second.end() ) {
          //The "other" nodes are also connected.  This is a triangle:
	  triangles++;
	}
      }
    }
    //Look at the wedges from the other end:
    this_node = this_edge->second;
    other1 = this_edge->first;
    emap_it = _node_to_edges.find(this_node);
    for( e2_it = emap_it->second.begin();
	 e2_it != emap_it->second.end();
	 e2_it++) {
      if( *e2_it != this_edge ) {
        //There is one more wedge:
        wedges++;
        //See if it is a triangle:
	Node* other = (*e2_it)->getOtherNode(this_node);
	map<Node*, ConnectedNodePSet>::const_iterator map_it;
	map_it = connection_map.find(other);
        if( map_it->second.find(other1) != map_it->second.end() ) {
          //The "other" nodes are also connected.  This is a triangle:
	  triangles++;
	}
      }
    }
  //We saw the triangles 2 times, once
  //from one "end" once from the other
  //reduce the number:
  triangles /= 2;
  return triangles;
}

void Network::getTrianglesWedges(int& triangles, int& wedges) const {

  /*
   * Iterate over all edges and count the number
   * of triangles and wedges seen.
   *
   * Each edge will be seen six times in a triangle
   * and twice in a wedge.
   * 
   */
  EdgeSet::const_iterator e_it, e2_it;
  map<Node*, EdgeSet>::const_iterator emap_it;
  triangles = 0;
  wedges = 0;
  for(e_it = edge_set.begin(); e_it != edge_set.end(); e_it++) {
    Edge* this_edge = *e_it;
    //Look at the wedges from one end:
    Node* this_node = this_edge->first;
    Node* other1 = this_edge->second;
    emap_it = _node_to_edges.find(this_node);
    for( e2_it = emap_it->second.begin();
	 e2_it != emap_it->second.end();
	 e2_it++) {
      if( *e2_it != *e_it ) {
        //There is one more wedge:
        wedges++;
        //See if it is a triangle:
	Node* other = (*e2_it)->getOtherNode(this_node);
	map<Node*, ConnectedNodePSet>::const_iterator map_it;
	map_it = connection_map.find(other);
        if( map_it->second.find(other1) != map_it->second.end() ) {
          //The "other" nodes are also connected.  This is a triangle:
	  triangles++;
	}
      }
    }
    //Look at the wedges from the other end:
    this_node = this_edge->second;
    other1 = this_edge->first;
    emap_it = _node_to_edges.find(this_node);
    for( e2_it = emap_it->second.begin();
	 e2_it != emap_it->second.end();
	 e2_it++) {
      if( *e2_it != *e_it ) {
        //There is one more wedge:
        wedges++;
        //See if it is a triangle:
	Node* other = (*e2_it)->getOtherNode(this_node);
	map<Node*, ConnectedNodePSet>::const_iterator map_it;
	map_it = connection_map.find(other);
        if( map_it->second.find(other1) != map_it->second.end() ) {
          //The "other" nodes are also connected.  This is a triangle:
	  triangles++;
	}
      }
    }
  }
  //We saw the triangles 6 times, reduce the number:
  triangles /= 6;
  //We saw edge wedge 2 times:
  wedges /= 2;
}

int Network::getWedgeCount() const {
  map<int, int> deg_dist = getDegreeDist();
  /*
   * W = n <k(k-1)/2> = n sum{ p_k k(k-1)/2 }
   */
  map<int,int>::const_iterator dit;
  int wedges = 0;
  for(dit = deg_dist.begin(); dit != deg_dist.end(); dit++) {
    int k, n_k;
    k = dit->first;
    n_k = dit->second;
    wedges += n_k * (k * (k - 1))/2;
  }
  return wedges;
}

bool Network::has(const Edge& edge) const {
    return ( 0 != getEdgePtr(edge));
}

bool Network::has(Node* node) const {
    return (node_set.find(node) != node_set.end());
}

int Network::incrementEdgeRefCount(Edge* edge) {
    map<Edge*, int>::iterator ref_it = _edge_ref_count.find(edge);
    if( ref_it != _edge_ref_count.end() ) {
      if( ref_it->second == 0 ) { cerr << "going from 0 -> 1:edge:"<< edge << endl; } 
      ref_it->second = ref_it->second + 1;
      return ref_it->second;
    }
    else {
      _edge_ref_count[edge] = 1;
      return 1;
    }
    return -1;
}

int Network::incrementNodeRefCount(Node* node) {
    map<Node*, int>::iterator ref_it = _node_ref_count.find(node);
    if( ref_it != _node_ref_count.end() ) {
      if( ref_it->second == 0 ) { cerr << "going from 0 -> 1:node:"<< node << endl; } 
      ref_it->second = ref_it->second + 1;
      return ref_it->second;
    }
    else {
      _node_ref_count[node] = 1;
      return 1;
    } 
    return -1;
}

void Network::printTo(ostream& out) const {

#if 1
   /*
    * This a really simple format:
    * node : neighbor1 neighbor2 ... neighborN <newline>
    */
  NodePSet::const_iterator i;
  ConnectedNodePSet::const_iterator j;
  map< Node*, ConnectedNodePSet >::const_iterator k;
  for(i = node_set.begin(); i != node_set.end(); i++) {
    k = connection_map.find( *i );
    out << (*i)->toString() << " :";
    if( k != connection_map.end() ) {
      for(j = k->second.begin(); j != k->second.end(); j++) {
        out << " " << (*j)->toString();
      }
    }
    out << endl;
  }
#endif
	
#if 0
    /*
     * This is for use with graphviz, an AT&T graph visualization package
     * I had to cast the add some text ("n") so as not to confuse graphviz.
     */
	
    NodePSet::const_iterator i;
    ConnectedNodePSet::const_iterator j;
    map< Node*, ConnectedNodePSet >::const_iterator k;
    string label;
    int node_count = 0;
    
    out << "graph g {" << endl;
    for(i = node_set.begin(); i != node_set.end(); i++) {
	label = (*i)->toString();
	if ( label != "" ) {
	  out << "n" << *i << " [label=\"" << label 
		  << "\",width=0.25,height=0.25,fontsize=9];" << endl;
	}
	else {
          out << "n" << *i << " [label=\"" << node_count
		  << "\",width=0.25,height=0.25,fontsize=9];" << endl;
	}
        k = connection_map.find( *i );
	if( k != connection_map.end() ) {
	    for(j = k->second.begin(); j != k->second.end(); j++) {
                out << "n" << *i << " -- " << "n" << *j << ";" << endl;
	    }
	}
	else {
          //This node has no connections:
	  out << "n" << *i << ";" << endl;
	}
	node_count++;
    }
    out << "}" << endl;
#endif
    
#if 0
    //This is for Tulip, a GPL graph viewer.
    //It MUST use integers to refer to nodes and edges.

    NodePSet::const_iterator i;
    for(i = node_set.begin(); i != node_set.end(); i++) {
	out << "(node " << (int)*i << ")" << endl;
    }
    EdgeSet::const_iterator j;
    int k = 1;
    for(j = edge_set.begin(); j != edge_set.end(); j++) {
        out << "(edge " << k++ << " " << (int)j->first << " " << (int)j->second << ")" << endl;
    }
#endif
    
}

void Network::readFrom(istream& in) {

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
        add( Edge(first, second) );
      }
    }
  }
	
}

int Network::remove(const Edge& edge) {
  Edge* e_p = getEdgePtr(edge);
  int ret = 0;
  if(e_p) {
    ret += remove(e_p);
  }
  return ret;
}

int Network::remove(Edge* e_p) {
  EdgeSet::iterator eit = edge_set.find(e_p);
  int return_val = 0;
  if( eit != edge_set.end() ) {
    //This edge is in the network	  
    list<INetworkMonitor*>::iterator nm_it;
    for(nm_it = _net_mon.begin(); nm_it != _net_mon.end(); nm_it++) {
      (*nm_it)->preEdgeRemove(e_p);
    }
    return_val = edge_set.erase( e_p );
    if( return_val != 0) { 
      connection_map[e_p->first].erase(e_p->second);
      connection_map[e_p->second].erase(e_p->first);
      _node_to_edges[e_p->first].erase(e_p);
      _node_to_edges[e_p->second].erase(e_p);
    
      for(nm_it = _net_mon.begin(); nm_it != _net_mon.end(); nm_it++) {
        (*nm_it)->postEdgeRemove(e_p);
      }
      //remove a reference to the edge:
      //This should be done last, other wise the postEdgeRemove may get
      //a deleted pointer
      decrementEdgeRefCount(e_p);
    }
  }
  return return_val;
}

int Network::remove(Node* node) {
  int removed_edges = 0;
  NodePSet::iterator nit = node_set.find(node);
  if( nit != node_set.end() ) {
    list<INetworkMonitor*>::iterator nm_it;
    for(nm_it = _net_mon.begin(); nm_it != _net_mon.end(); nm_it++) {
      (*nm_it)->preNodeRemove(node);
    }
    node_set.erase(node);
    EdgeSet::iterator e_it;
    map<Node*, EdgeSet>::iterator ne_it = _node_to_edges.find(node);
    if( ne_it != _node_to_edges.end() ) {
      for(e_it = ne_it->second.begin(); e_it != ne_it->second.end(); e_it++) {
        removed_edges += remove( *e_it );
      }
    }
    _node_to_edges.erase(node);
    connection_map.erase(node);
    for(nm_it = _net_mon.begin(); nm_it != _net_mon.end(); nm_it++) {
      (*nm_it)->postNodeRemove(node);
    }
    //Get rid of one of the references to this node, possibly deleting it.
    decrementNodeRefCount(node);
  }
  return removed_edges;
}

int Network::remove(const NodePSet& nodes) {
    NodePSet::iterator it;
    int ret = 0;
    for(it = nodes.begin(); it != nodes.end(); it++) {
        ret += remove(*it);
    }
    return ret;
}

void Network::remove(INetworkMonitor* nm) {
  list<INetworkMonitor*>::iterator nmit;
  for(nmit = _net_mon.begin(); nmit != _net_mon.end(); nmit++) {
    if( *nmit == nm ) {
      break;
    }
  }
  if( nmit != _net_mon.end() ) {
    //Just remove this guy:
    _net_mon.erase(nmit);
  }
}

int Network::removeAndCluster(Node* n) {
    const ConnectedNodePSet& set = getNeighbors(n);

    //Connect all the neighbors
    ConnectedNodePSet::iterator i,j;
    for(i = set.begin(); i != set.end(); i++) {
      for(j=i; j != set.end(); j++) {
        add( Edge(*i,*j) );
      }
    }
    //Now return the node
    return remove(n);
}

int Network::removeAndCluster(const NodePSet& nodes) {
    NodePSet::iterator it;
    int ret = 0;
    for(it = nodes.begin(); it != nodes.end(); it++) {
        ret += removeAndCluster(*it);
    }
    return ret;
}

bool Network::operator<(const Network& aNet) const {
    //Make sure networks with smaller nodes are <
    if( node_set.size() != aNet.node_set.size() ) {
        return (node_set.size() < aNet.node_set.size());
    }
    //okay, these two networks have the same number of nodes, but how about edges:
    if( edge_set.size() != aNet.edge_set.size() ) {
        return (edge_set.size() < aNet.edge_set.size() );
    }
    
    //If two networks are the same size,
    //but have different set of nodes, let the STL sort it out:
    if( node_set != aNet.node_set ) {
        return (node_set < aNet.node_set );
    }
    if (edge_set != aNet.edge_set ) {
        return (edge_set < aNet.edge_set );
    }
    if ( connection_map != aNet.connection_map ) {
        return (connection_map < aNet.connection_map);
    }
    return false;
}

Network& Network::operator=(const Network& aNet) {
  if( this != &aNet) {
      clear();
      node_set = aNet.node_set;
      edge_set = aNet.edge_set;
      connection_map = aNet.connection_map;
      _node_to_edges = aNet._node_to_edges;
      
      //Bump the reference count on each of the nodes:
      NodePSet::const_iterator n_it;
      for(n_it = node_set.begin(); n_it != node_set.end(); n_it++) {
          incrementNodeRefCount( *n_it );
      }
      EdgeSet::const_iterator e_it;
      for(e_it = edge_set.begin(); e_it != edge_set.end(); e_it++) {
          incrementEdgeRefCount( *e_it );
      }
  }
  return (*this);
}
bool Network::Merge(const Network& aNet, int aStart){
 //check if the ranges are correct
	if (this->getNodes().size() < ((aStart-1) + aNet.getNodes().size())){
		cerr<<" the sizes do not match in merge";
		return(false);
	}
	set<Node*>::iterator start_iterator,i,j;
	start_iterator = node_set.begin();
	for( int icounter = 0; icounter<(aStart-1); icounter++)
		start_iterator ++;
	i = start_iterator;
	j = aNet.node_set.begin();
	for(;j != aNet.node_set.end(); j++){
	// get neihbors of j find them and add proper edges
		ConnectedNodePSet j_neighbors = aNet.getNeighbors(*j);
		ConnectedNodePSet::const_iterator n_it;
	 	for(n_it = j_neighbors.begin(); n_it!=j_neighbors.end(); n_it++){
			NodePSet::const_iterator m_it = start_iterator;
			NodePSet::const_iterator p_it ;
			for( p_it = aNet.node_set.begin();((*p_it) != (*n_it)); p_it++){
				m_it++;
			}
			add(Edge(*m_it,*i));
		}
		i++;
	}
	return(true);
}





void Network::readFromGDL(std::istream& in){
  SuperString line;
  map<string, Node*> name_map;
  list<string> edge_src;
  list<string> edge_dst;
  int node=0, edge=0;
  Node* temp1 = 0;
  Node* temp2 = 0;

  while( getline(in, line, '\n') ) 
  {
    if( line[0] == '/' && line[1] == '/' ) continue;

    vector<SuperString> result = line.split(" ");
    vector<SuperString>::iterator sit;
    string nodename="", source="", target="";

    for (sit = result.begin(); sit != result.end(); sit++)
    {
      // note: should be able to handle node/edge definitions that span multiple lines now
      // note: doesn't handle multiword titles/sources/destinations, but they shouldn't exist anyway

      if (*sit == "node:")
        node = 1;
      else if (*sit == "edge:" || *sit == "nearedge:")
        edge = 1;
      else if (node == 1 && *sit == "title:")
      {
        sit++;
        nodename = *sit;

        if (name_map.find(nodename) == name_map.end()) // if new node...
        {
          temp1 = new Node(nodename);
          name_map[nodename] = temp1;
          add( temp1 );
          //cout<<"Created node "<<nodename<<endl;
        }

        nodename = "";
        node = 0;
      }
      else if (edge == 1 && (*sit == "targetname:" || *sit == "target:"))
      {
        sit++;
        target = *sit;
      }
      else if (edge == 1 && (*sit == "sourcename:" || *sit == "source:"))
      {
        sit++;
        source = *sit;
      }

      if (edge == 1 && source != "" && target != "")
      {
        edge_src.push_front(source);
        edge_dst.push_front(target);
        source = "";
        target = "";
        edge = 0;
      }

    }
  }

  list<string>::iterator i = edge_src.begin(), j = edge_dst.begin();

  while (i != edge_src.end())
  {
    //cout<<"Created edge from "<<*i<<" to "<<*j<<endl;
    temp1 = name_map[*i];
    temp2 = name_map[*j];
    add( Edge(temp1, temp2) );
    i++;
    j++;
  }

	
}

void Network::printToGDL(ostream& out) const {

  /*
   * This GDL output is for use with aiSee/VCG graph tools
   */
	
  NodePSet::const_iterator i;
  ConnectedNodePSet::const_iterator j;
  map< Node*, ConnectedNodePSet >::const_iterator k;
  string label;
  int node_count = 0;
    
  out<<"graph: {\n";
  out<<"  title: \"untitled\"\n";
  out<<"  colorentry 16: 161 215 244\n";
  out<<"  color: lightblue\n";
  out<<"  layoutalgorithm: forcedir\n";
  out<<"  repulsion: 150\n";
  out<<"  attraction: 150\n";
  out<<"\n";

  for(i = node_set.begin(); i != node_set.end(); i++) {
	  label = (*i)->toString();
    out<<"  node: { title: \""<<label<<"\" label: \""<<label<<"\" }\n";
	  //out << 'n' << *i << " [label=\"" << label << "\",width=0.25,height=0.25,fontsize=9];" << endl;

    k = connection_map.find( *i );
	  if( k != connection_map.end() ) {
	    for(j = k->second.begin(); j != k->second.end(); j++) {
        out<<"  edge: { sourcename: \""<<label<<"\" targetname: \""<<(*j)->toString()<<"\" }\n";
        //out << 'n' << *i << " -> " << 'n' << *j << ";" << endl;
      }
    }
    node_count++;
  }
  out << "}\n" << endl;    
}
