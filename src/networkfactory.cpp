#include <networkfactory.h>

using namespace std;
using namespace Starsky;

NetworkFactory::NetworkFactory() {

  _nf = new NodeFactory();
  _ef = new EdgeFactory();
}

NetworkFactory::NetworkFactory(NodeFactory* nf, EdgeFactory* ef) : _nf(nf), _ef(ef) 
{

}

NetworkFactory::~NetworkFactory()
{
  delete _nf;
  _nf = 0;
  delete _ef;
  _ef = 0;
}

Network* NetworkFactory::create(istream& in)
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
    //There is a list of second nodes:
    result = result[1].split(" ");
    vector<SuperString>::iterator sit;
    for(sit = result.begin();
	sit != result.end();
	sit++) {
      second = 0;
      second = _nf->create( *sit );
      net->add( second );
      Edge* e = 0;
      if( result.size() > 2 ) {
        e = _ef->create(first, second, result[2]);
      }
      else {
        e = _ef->create(first, second);
      }
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
