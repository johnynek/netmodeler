#include "magnetmessage.h"

using namespace Starsky;
using namespace std;

MagnetMessage::MagnetMessage(Random& r, double p, int ttl) : _rand(r),
	                                                     _p(p),
							     _ttl(ttl)
{
  forgetVisitedNodes();
}

void MagnetMessage::visit(Node* n, Network& net)
{
    map<int, Network::NodePSet > to_visit;
    map<int, Network::NodePSet >::iterator tv_it;
    Network::ConnectedNodePSet neighbors;
    Network::ConnectedNodePSet::iterator n_it;
    Network::NodePSet::iterator a_it;
    int this_distance, routes, rand_node;

    to_visit[0].insert(n);
    _visited_nodes.insert(n);
    
    //We loop through at each TTL:
    tv_it = to_visit.begin();
    while( tv_it != to_visit.end() &&
		    ( (tv_it->first < _ttl) || (_ttl == -1) ) )
    {
        this_distance = tv_it->first + 1;
        //Here are all the nodes at this distance:
        for( a_it = tv_it->second.begin();
	     a_it != tv_it->second.end();
	     a_it++)
	{
            //Now look for a random node to go to:
	    const Network::ConnectedNodePSet& neighbors
		    = net.getNeighbors( *a_it );
	    //Make sure there are some neighbors to go to:
	    if( neighbors.size() > 0 ) {
	      Node* vnode = 0;
	      if( _rand.getBool( _p ) ) {
                //select the neighbor with maximum degree
		n_it = neighbors.begin();
		int max_deg = net.getDegree(*n_it);
		vnode = *n_it;
		while( n_it++ != neighbors.end() ) {
                  if(max_deg < net.getDegree(*n_it)) {
                    max_deg = net.getDegree(*n_it);
		    vnode = *n_it;
		  }
		}
	      }
	      else {
                //select a random node:
	        rand_node = _rand.getInt( neighbors.size() - 1 );
	        n_it = neighbors.begin();
	        while( rand_node-- > 0 ) { n_it++; }
		vnode = *n_it;
	      }
	      //Visit the randomly selected node:
	      to_visit[this_distance].insert( vnode );
	      _visited_nodes.insert( vnode );
	      //We must cross exactly one edge to visit the above node:
	      _crossed_edges++;
	    }
        }
        to_visit.erase(tv_it);
        tv_it = to_visit.begin();
    }
}
