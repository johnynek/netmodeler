#include <weightednetworkfactory.h>

using namespace std;
using namespace Starsky;

WeightedNetworkFactory::WeightedNetworkFactory()
{
  _nf = new NodeFactory();
  _ef = new WeightedEdgeFactory();
}

WeightedNetworkFactory::WeightedNetworkFactory(NodeFactory* nf, EdgeFactory* ef) : NetworkFactory(nf,ef)
{

}

Network* WeightedNetworkFactory::create(istream& in)
{
  Network* net = new Network();
  SuperString line;
  while( getline(in, line, '\n') ) {
    if( line[0] == '#' ) { //This is a comment
      continue;
    }
    vector<SuperString> result = line.split(" : ");
    Node* first = 0;
    Node* second = 0;
    first = _nf->create(result[0]);
    net->add( first ); 
    if( result.size() == 1 ) { //There was no neighbors
      continue;
    }
    const Network::EdgeSet& edges = net->getEdges(first);
    //There is a list of second nodes:
    result = result[1].split(" ");
    vector<SuperString>::iterator sit;
    for(sit = result.begin();
	sit != result.end();
	sit++) {
      second = 0;
      second = _nf->create( *sit );
      net->add( second );
      //Find any existing edge for this Node:
      Network::EdgeSet::const_iterator e_it;
      Edge* existing = 0;
      for(e_it = edges.begin(); e_it != edges.end(); e_it++) {
        if( (*e_it)->connects(first, second) ) {
          existing = *e_it;
	  break;
	}
      }
      double weight = 0.0;
      if( existing != 0 ) {
        weight = existing->getWeight();
      }
      //Delete the existing edge:
      net->remove(existing);
      Edge* e = 0;
      if( result.size() > 2 ) {
	weight += atof( result[2].c_str() );
      }
      else {
        //The weight must be 1.0:
	weight += 1.0;
      }
      stringstream attr;
      attr << weight;
      e = _ef->create(first, second, attr.str() );
      //Add the edge:
      if( e ) {
	/**
	 * Remember: we have to add references to edges, 
	 * not pointers.  This is because we don't want to allow
	 * multiple edges between nodes (for now).
	 */
        net->add( *e );
      }
      delete e;
    }
  }
  return net;
}
