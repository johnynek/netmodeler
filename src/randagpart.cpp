#include "randagpart.h"

//#define DEBUG

using namespace std;
using namespace Starsky;

RandAgPart::RandAgPart(Random& rand, RandAgPart::Method meth, double param):
	_rand(rand), _method(meth), _param(param)
{

}

void RandAgPart::initRandomCom(const Network& net) {
  _com_order.clear();
  _com_to_node.clear();
  const Network::NodePSet& node_set = net.getNodes();
  _com_to_node.insert(_com_to_node.begin(), node_set.begin(), node_set.end());
  int count = node_set.size();
  _com_order.resize( count );
  for(int i = 0; i < count; i++) {
    _com_order[i] = i;
  }
  //Permute the list:
  for(int i = count - 1; i > 0; i--) {
       //Get a random element from 0..i
       int swap_with = _rand.getInt(i);
       if( swap_with != i ) {
         int tmp = _com_order[swap_with];
         _com_order[swap_with] = _com_order[i];
         _com_order[i] = tmp;
     }
  }
  _com_pos = 0;
}

int RandAgPart::getCommunities(const Network& net, std::vector<double>& q,
                   std::vector< std::pair<int,int> >& joins)
{
  if( _method == RandomStart || _method == NoisyJoin ) {
  }
  else {
    /* The other methods don't use random community selection */
  }
  initRandomCom(net);
  return NewmanCom::getCommunities(net,q,joins);
}

double RandAgPart::getNextJoin(const Network& net,
                               const std::map<Node*, int>& node_community,
		               const vector< vector<double> >& e_ij,
			       const vector< double >& a_i,
			       int& join1, int& join2)
{
  //cout << "starting get next join" << endl;
  double delta_q, tmp_delta;
  if( _method == RandomStart ) {
    join1 = getRandomCommunity();
    
    bool got_first = false;
    int com1 = -1;
    int com2 = -1;

    const Network::EdgeSet& edge_set = net.getEdges();
    map<Node*, int>::const_iterator com1_it, com2_it;
    Network::EdgeSet::const_iterator e_it;
    for(e_it = edge_set.begin();
        e_it != edge_set.end();
        e_it++) {
      com1_it = node_community.find( (*e_it)->first );
      com1 = com1_it->second;
      com2_it = node_community.find( (*e_it)->second );
      com2 = com2_it->second;
      
      //Don't join a community with itself, and make sure this edge
      //touches the randomly selected community
      if( com1 != com2 && ( (com1 == join1) || (com2 == join1) ) ) {
        tmp_delta = 2 * (e_ij[com1][com2] - a_i[com1] * a_i[com2] );
     //   cout << "tmp_delta: " << tmp_delta << endl;
        if( (!got_first) || ( tmp_delta > delta_q ) ) {
          delta_q = tmp_delta;
  	  if( com1 != join1 ) { join2 = com1; }
	  else { join2 = com2; }
	  got_first = true;
        }
      }
    }
   // cout << "Max delta_q: " << delta_q << endl;
  }
  else if( _method == NoisyJoin ) {
    if( _rand.getBool( _param ) ) {
      //Noisy join:
      join1 = getRandomCommunity();
      join2 = getRandomCommunity(join1);
      delta_q = 2 * (e_ij[join1][join2] - a_i[join1] * a_i[join2] );
    }
    else {
      delta_q = NewmanCom::getNextJoin(net, node_community, e_ij, a_i, join1, join2);
    }
    
  }
  else if( _method == PrefJoin ) {
    /**
     * In the PrefJoin method we find a q(j) for each
     * join, where q(j) = delta_q(j) - delta_qmin
     * Thus, q(j) >= 0.  Then we select this join
     * with probability proportional to q(j).
     */
    const Network::EdgeSet& edge_set = net.getEdges();
    map<Node*, int>::const_iterator com1_it, com2_it;
    Network::EdgeSet::const_iterator e_it;
    int com1, com2;
    double min_q, tmp_delta;
    double total_q = 0.0;
    int joins = 0;
    bool got_first = false;
    for(e_it = edge_set.begin();
        e_it != edge_set.end();
        e_it++) {
      com1_it = node_community.find( (*e_it)->first );
      com1 = com1_it->second;
      com2_it = node_community.find( (*e_it)->second );
      com2 = com2_it->second;
      
      //Don't join a community with itself
      if( com1 != com2 ) {
        tmp_delta = 2 * (e_ij[com1][com2] - a_i[com1] * a_i[com2] );
        if( (!got_first) || ( tmp_delta < min_q ) ) {
	  min_q = tmp_delta;
	  got_first = true;
        }
	joins++;
      }
    }
    //Now we shift all the values by the minimum value:
    total_q = 0.0;
    for(e_it = edge_set.begin();
        e_it != edge_set.end();
        e_it++) {
      com1_it = node_community.find( (*e_it)->first );
      com1 = com1_it->second;
      com2_it = node_community.find( (*e_it)->second );
      com2 = com2_it->second;
      
      //Don't join a community with itself
      if( com1 != com2 ) {
        tmp_delta = 2 * (e_ij[com1][com2] - a_i[com1] * a_i[com2] );
	total_q += pow(tmp_delta - min_q, _param);
      }
    }
    double q_rand = total_q * _rand.getDouble01(); 
    double q_samp = 0.0;
    //When we pass the join that puts q_samp > q_rand, we stop:
    for(e_it = edge_set.begin();
        e_it != edge_set.end();
        e_it++) {
      com1_it = node_community.find( (*e_it)->first );
      com1 = com1_it->second;
      com2_it = node_community.find( (*e_it)->second );
      com2 = com2_it->second;
      
      //Don't join a community with itself
      if( com1 != com2 ) {
        tmp_delta = 2 * (e_ij[com1][com2] - a_i[com1] * a_i[com2] );
	q_samp += pow(tmp_delta - min_q, _param);
	if( q_samp > q_rand ) {
          //This is the pair of communities to join
  	  if( com1 < com2 ) { join1 = com1; join2 = com2; }
	  else { join1 = com2; join2 = com1; }
	  delta_q = tmp_delta;
          break;
	}
      }
    }
    //Now we have the communities:
  }
  if( join2 < join1 ) {
    int temp = join2;
    join2 = join1;
    join1 = temp;
  }
  /*
   * join2 is being merged into join1, so it should
   * not be selected anymore
   */
  removeCommunity(join2);
  //cout << "ending get next join (" << join1 << "," << join2 << ") delta_q=" << delta_q << endl;
  return delta_q;
}

int RandAgPart::getRandomCommunity(int avoid)
{
  //cout << "about to get com" << endl;
  int com;
  do {
    _com_pos = _rand.getInt( _com_order.size() - 1 );
    com = _com_order[ _com_pos ];
  }
  while( (_com_to_node[ _com_pos ] == 0) || (com == avoid) );
  //cout << "got: " << com << endl;
  return com;
}

void RandAgPart::removeCommunity(int com)
{
  _com_to_node[ com ] = 0;
}
