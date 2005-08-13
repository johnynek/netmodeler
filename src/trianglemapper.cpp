#include "trianglemapper.h"

#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

using namespace Starsky;

void TriangleMapper::map(Network* net)
{
  Network::EdgeSet::iterator eit;
  Network::EdgeSet to_remove;
  const Network::EdgeSet& all_edges = net->getEdges();
  FOREACH( eit, all_edges )  {
    if( net->getDegree( (*eit)->first ) == 1 ||
        net->getDegree( (*eit)->second ) == 1 ) {
      //This is a dead-end, keep it.
    }
    else {
      int tri = net->getTriangles( *eit );
      if( tri == 0 ) {
        //This edge is not in any triangles
        to_remove.insert( *eit );
      }
    }
  }
  //Actually remove the edges:
  FOREACH( eit, to_remove ) {
    net->remove( *eit );
  }
}
