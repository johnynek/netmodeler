#include <uniformnodeselector.h>

using namespace Starsky;
using namespace std;

void UniformNodeSelector::postAdd(Network* net)
{
  _my_net = net;
  _nodes.clear();
  _nodes.insert( _nodes.begin(), _my_net->getNodes().begin(), _my_net->getNodes().end() );
}

void UniformNodeSelector::postNodeAdd(Node* add, bool success)
{
  if( success ) {
    //This is a new node:
    _nodes.push_back( add );
  }
}

void UniformNodeSelector::postNodeRemove(Node* add, bool success)
{
  //Remove this node:
  if( success ) {
    vector<Node*>::iterator nit;
    for(nit = _nodes.begin(); nit != _nodes.end(); nit++) {
      if( *nit == add ) {
        _nodes.erase(nit);
        break;
      }
    }
  }
}

Node* UniformNodeSelector::select()
{
  return select(0);
}

Node* UniformNodeSelector::select(Node* avoid)
{
  Node* ret_val = 0;
  if( _nodes.size() == 0 ) {
    ret_val = 0;
  }
  else if( _nodes.size() == 1 ) {
    if( _nodes[0] != avoid ) {
      ret_val = _nodes[0];
    }
    else {
      ret_val =  0;
    }
  }
  else {
    while( ret_val == 0 ) {
      int rand_int = _rand.getInt( _nodes.size() - 1 );
      ret_val = _nodes[rand_int];
      if( ret_val == avoid ) { ret_val = 0; }
    }
  }
  return ret_val;
}
