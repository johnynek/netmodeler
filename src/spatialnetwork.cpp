#include "spatialnetwork.h"

using namespace Starsky;
using namespace std;

SpatialNetwork::SpatialNetwork(int nodes,
		               Random& rand,
			       int cons,
			       int dim) : Network(),
			                        _dim(dim),
						_cons(cons)
{
    while( nodes-- > 0 ) {
        add(new SpatialNode(rand,_dim));
    }

    NodePSet::iterator n_it1, n_it2;
    SpatialNode *start_node = 0, *end_node = 0;
    priority_queue<double> dist_queue;
    multimap<double, SpatialNode*> dist_to_node;
    multimap<double, SpatialNode*>::iterator mm_it;
    double this_dist = 0.0;
    
    for( n_it1 = node_set.begin(); n_it1 != node_set.end(); n_it1++) {
	while( dist_queue.empty() == false) {
            dist_queue.pop();	
	}
	int i;
	this_dist = (double)dim;
	end_node = 0;
	dist_to_node.clear();
	for(i = 0; i < cons; i++) {
            dist_queue.push(this_dist); //the largest distance is sqrt(d) which is less than d.
	    dist_to_node.insert( pair<double,SpatialNode*>(this_dist,end_node) );
	}
	
	start_node = dynamic_cast<SpatialNode*>(*n_it1);
	if( start_node != 0 ) {
	  for(n_it2 = node_set.begin(); n_it2 != node_set.end(); n_it2++) {
            end_node = dynamic_cast<SpatialNode*>(*n_it2);
	    if( (start_node != end_node) && (end_node != 0) ) {
              this_dist = start_node->getDistanceTo( end_node );
	      if( this_dist > 0 ) {
		  if( this_dist < dist_queue.top() ) {
		      //Remove the old node:
		      mm_it = dist_to_node.find( dist_queue.top() );
		      dist_to_node.erase(mm_it);
                      dist_queue.pop();
		      
		      //Add the new one:
		      dist_to_node.insert( pair<double,SpatialNode*>(this_dist,end_node) );
		      dist_queue.push(this_dist);
		  }
	      }
	    }
	  }
	}
	//Now we add the edges to all the nodes:
	for(mm_it = dist_to_node.begin(); mm_it != dist_to_node.end(); mm_it++) {
            add(Edge(start_node, mm_it->second));
	}
    }
}
