#include "npartitenetwork.h"

using namespace std;
using namespace Starsky;

bool NPartiteNetwork::add(int type, Node* node)
{
  _type_to_node_map[type].insert(node);
  return Network::add(node);
}

const Network::NodePSet& NPartiteNetwork::getNodes(int type) const
{
  map<int,Network::NodePSet>::const_iterator it = _type_to_node_map.find( type );
  if( _type_to_node_map.end() == it) {
      return Network::_empty_nodeset;
  }
  return it->second;
}

NPartiteNetwork& NPartiteNetwork::operator=(const NPartiteNetwork& net)
{
  this->operator=(net);
  _type_to_node_map = net._type_to_node_map;
  return *this;
}

int NPartiteNetwork::remove(Node* n)
{
  //We don't ask for the type here, but we assume that there are not
  //many types.  We are optimizing towards networks with large number
  //of nodes, but a low number of types.
  map<int, Network::NodePSet>::iterator it;
  for( it = _type_to_node_map.begin(); it != _type_to_node_map.end(); it++)
  {
    it->second.erase(n);
  }
  return Network::remove(n);
}
