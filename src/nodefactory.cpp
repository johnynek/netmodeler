#include <nodefactory.h>

using namespace Starsky;

Node* NodeFactory::create(const std::string& str_node)
{
    Node* result = getNode(str_node);
    if( result == 0 ) {
      //This is a new node:
      result = newNode(str_node);
      _str_to_node[ str_node ] = result;
    }
    return result;
}

Node* NodeFactory::getNode(const std::string& str_node)
{
    Node* result = 0;
    std::map<std::string, Node*>::iterator sit;
    sit = _str_to_node.find(str_node);
    if( sit != _str_to_node.end() ) {
      result = sit->second;
    }
    return result;
}
