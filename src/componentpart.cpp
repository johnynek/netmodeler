#include "componentpart.h"

using namespace std;
using namespace Starsky;

set<Network*>* ComponentPart::partition(const Network& input)
{
    set<Network*>* out = new set<Network*>();
    Network* tmp_net = 0;
    
    Network::NodePSet to_check, checked;
    Network::NodePSet::const_iterator i;
    Network::NodePSet::iterator check_it;
    
    Network::ConnectedNodePSet::const_iterator n_it;
    
    Network::EdgeSet tmp_edges;
    Network::EdgeSet::const_iterator e_it;
    
    //For all nodes
    const Network::NodePSet& node_set = input.getNodes();
    for(i = node_set.begin(); i != node_set.end(); i++) {
	//Check to make sure we have not already identified this component:
	if( checked.find( *i ) == checked.end() ) {
            to_check.insert( *i );
            check_it = to_check.begin();
	    //Here we make a new component:
	    tmp_net = new Network();
            while( check_it != to_check.end() ) {
		tmp_net->add( *check_it );
                tmp_edges = input.getEdges( *check_it );
	        for(e_it = tmp_edges.begin(); e_it != tmp_edges.end(); e_it++) {
                    tmp_net->add( *e_it );
	        }
	        for(n_it = input.getNeighbors( *check_it ).begin();
		    n_it != input.getNeighbors( *check_it ).end();
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
	    out->insert( tmp_net );
        }
	else {
	}
    }
    return out;
}
