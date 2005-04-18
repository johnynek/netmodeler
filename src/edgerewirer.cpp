#include <edgerewirer.h>
#include <uniformnodeselector.h>

#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

using namespace Starsky;
using namespace std;

EdgeRewirer::EdgeRewirer(EdgeFactory& ef, double p, Random& rand): _ef(ef),
								   _prob(p),
								   _rand(rand)
{
  //Make the default Node selectors:
  _startsel = new UniformNodeSelector(_rand);
  _endsel = _startsel;
}

EdgeRewirer::EdgeRewirer(EdgeFactory& ef, double p, Random& rand,
		INodeSelector* startsel,
		INodeSelector* endsel) : _ef(ef), _prob(p), _rand(rand)
{
  _startsel = startsel;
  _endsel = endsel;
  if( _endsel == 0) {
    _endsel = startsel;
  }
}

EdgeRewirer::~EdgeRewirer()
{
  if( _startsel != _endsel ) {
    delete _endsel;
    _endsel = 0;
  }
  delete _startsel;
  _startsel = 0;
}

void EdgeRewirer::map(Network* net)
{
  if( _endsel != _startsel ) {
    _endsel->selectFrom( net );
  }
  _startsel->selectFrom( net );

  const Network::EdgeSet& edges = net->getEdges();
  Network::EdgeSet::const_iterator eit;
  set<Edge*> edges_to_rewire;
  FOREACH(eit, edges)
  {
    if( _rand.getBool(_prob) ) {
      //Rewire this edge:
      edges_to_rewire.insert( *eit );
    }
  }
  set<Edge*>::iterator reit;
  FOREACH(reit, edges_to_rewire)
  {
    string attrs = (*reit)->getAttributes();
    net->remove( *reit );
    Node* start = _startsel->select();
    Node* end = _endsel->select(start);
  //  cout << "(" << start << ", " << end << ")" << endl;
    Edge* new_edge = _ef.create(start, end, attrs);
    net->add(new_edge); 
  }
}
