#include "mergenetwork.h"

using namespace Starsky;
using namespace std;

MergeNetwork::MergeNetwork(const Network& seed,
		           Random& r,
			   int cons) : _rand(r), _cons(cons)
{
  Network::operator=(seed);
  //We need to fill up the node vector:
  _node_vec.insert(_node_vec.begin(), node_set.begin(), node_set.end() );
}

void MergeNetwork::incrementTime(int steps)
{
  while( steps-- > 0) {
    //Get the current number of nodes that we can connect to
    int nodes = node_set.size();
    int index0 = _rand.getInt(nodes - 1);
    //Don't choose the previous node:
    int index1 = _rand.getInt(nodes - 2);
    if( index1 == index0 ) {
      index1 = (index1 + 1 ) % nodes;
    }
    //Give index0 all index1's neighbors:
    Node* n0 = _node_vec[index0];
    Node* n1 = _node_vec[index1];
    const ConnectedNodePSet& neighbors = getNeighbors(n1);
    ConnectedNodePSet::const_iterator n_it;
    for(n_it = neighbors.begin(); n_it != neighbors.end(); n_it++)
    {
      if( n0 != *n_it ) {
        add( Edge(n0, *n_it) );
      }
    }
    //Remove all n1's edges:
    EdgeSet e = _node_to_edges[n1];
    EdgeSet::iterator e_it;
    for(e_it = e.begin(); e_it != e.end(); e_it++)
    {
      remove( *e_it );
    }
    //Give n1 some new edges:
    NodePSet new_neighbors;
    while( new_neighbors.size() < _cons )
    {
      Node* neighbor = _node_vec[ _rand.getInt(nodes - 1) ];
      if( neighbor != n1 ) {
        new_neighbors.insert(neighbor);
      }
    }
    //We have the neighbors, now we just make the edges:
    NodePSet::iterator neigh_it;
    for(neigh_it = new_neighbors.begin();
	neigh_it != new_neighbors.end();
	neigh_it++)
    {
      add( Edge(n1, *neigh_it) );
    }
  }

}
