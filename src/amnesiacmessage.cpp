#include "amnesiacmessage.h"

using namespace Starsky;
using namespace std;

AmnesiacMessage::AmnesiacMessage(Random& r,
				       int ttl)
: _rand(r), _ttl(ttl) {
  forgetVisitedNodes();
}

/**
 * We carefully avoid recursion in the below since it could
 * lead to an enormous memory requirement.  The memory of
 * the below function will be at most the size of the network
 */

void AmnesiacMessage::visit(Node* n, Network& _my_net) {
	
   Network::ConnectedNodePSet neighbors;
   set< pair<Node*,int> > to_visit;
   int new_ttl = _ttl, n_count = 0, rand_neighbor;

   Node* last_node = 0;
   
   to_visit.insert( pair<Node*,int>(n,_ttl) );
   set< pair<Node*,int> >::iterator j = to_visit.begin();
   Network::ConnectedNodePSet::const_iterator i;
   while( j != to_visit.end() ) {
       if( j->second > 0 ) {
           //We don't forward messages with 0 TTL and we never forward TTL == -1
           neighbors = _my_net.getNeighbors( j->first );
           n_count = neighbors.size();
	   //See if we have any neighbors to go to:
	   if( n_count > 0 ) {
	     _rand.setIntRange(n_count - 1);
	     //Get a random edge
	     rand_neighbor = _rand.getInt();
	     i = neighbors.begin();
	     for(int k = 0; k < rand_neighbor; k++) {
	         i++;
	     }
	     //We know the TTL was > 0, so new_ttl is a legitimate number
	     new_ttl = j->second - 1; 
	     to_visit.insert( pair<Node*,int>(*i,new_ttl) ); 
	     //We must cross one edge to visit the above node:
	     _crossed_edges++;
	   }
	   last_node = j->first;
       }
       //This will invalidate the j iterator:
       to_visit.erase( j );
       //So we reset it:
       j = to_visit.begin();
   }
   _visited_nodes.insert( last_node );
}
