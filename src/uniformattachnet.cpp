#include "uniformattachnet.h"

using namespace Starsky;
using namespace std;

UniformAttachNet::UniformAttachNet(const Network& seed,
		             Random& r,
			     int cons) : 
			                 _connections(cons),
					 _rand(r)
{
    Network::operator=(seed);
    //Load the nodes into a vector
    _nodes.insert( _nodes.begin(), getNodes().begin(), getNodes().end() ); 
}

bool UniformAttachNet::add(Node* n)
{
  _nodes.push_back(n);
  return Network::add(n);
}

Node* UniformAttachNet::findPartnerFor(Node* n)
{
    //Don't include the neighbors:
    NodePSet::const_iterator nit;
    const NodePSet& neighbors = getNeighbors(n);
    int index = _rand.getInt( _nodes.size() - 1 );
    Node* partner = _nodes[ index ];
    int attempts = _nodes.size();
    while( ((partner == n)
           || (neighbors.find(partner) != neighbors.end() ))
	   && (attempts > 0) )
    {
      index = (index + 1) % _nodes.size();
      partner = _nodes[ index ];
      attempts--;
    }
    if( attempts == 0 ) {
      cerr << "Count not find a partner for n: " << n << endl;
    }
    return partner;
}


int UniformAttachNet::remove(Node* n)
{
  _nodes.push_back(n);
  vector<Node*>::iterator n_it = _nodes.begin();
  while( *n_it != n && (n_it != _nodes.end()) )
  {
    n_it++;
  }
  if( *n_it == n ) { _nodes.erase( n_it ); }
  return Network::remove(n);
}
