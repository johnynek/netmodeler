#include "node.h"
using namespace Starsky;

//Initialize the node count to zero
int Node::_node_count = 0;

Node::Node()
{
  std::stringstream s;
  s << _node_count;
  _node_count++;
  _name = s.str();
}
