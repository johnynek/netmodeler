#include "newmancom.h"

using namespace std;
using namespace Starsky;

double NewmanCom::getNextJoin(const Network& net,
                               const std::map<Node*, int>& node_community,
		               const vector< vector<double> >& e_ij,
			       const vector< double >& a_i,
			       int& join1, int& join2)
{
    double delta_q, tmp_delta;
    bool got_first = false;
    //It is important to initialize the communities with
    //sane values.  If (for some reason) there are no edges
    //between these communities,
    if( node_community.size() > 1 ) {
      map<Node*, int>::const_iterator tmpit;
      tmpit = node_community.begin();
      join1 = tmpit->second;
      tmpit++;
      join2 = tmpit->second;
      //Make sure join1 <= join2:
      if( join2 < join1 ) { int tmp = join1; join1 = join2; join2 = tmp; }
    }
    else {
      join1 = node_community.begin()->second;
      join2 = join1;
    }
    delta_q = 0.0;
    const Network::EdgeSet& edge_set = net.getEdges();
    if( edge_set.size() == 0 ) {
      //cerr << "No edges to join" << endl;
    } 
    int com1, com2;
    map<Node*, int>::const_iterator com1_it, com2_it;
    Network::EdgeSet::const_iterator e_it;
    for(e_it = edge_set.begin();
        e_it != edge_set.end();
        e_it++) {
      com1_it = node_community.find( (*e_it)->first );
      com2_it = node_community.find( (*e_it)->second );
      com1 = com1_it->second;
      com2 = com2_it->second;
      if( com1 != com2 ) {
        tmp_delta = 2 * (e_ij[com1][com2] - a_i[com1] * a_i[com2] );
        if( (!got_first) || ( tmp_delta > delta_q ) ) {
          //Make sure join1 <= join2:
          if( com1 < com2 ) { join1 = com1; join2 = com2; }
          else { join1 = com2; join2 = com1; }
          delta_q = tmp_delta;
	  got_first = true;
        }
      }
    }
    
    return delta_q;
}
