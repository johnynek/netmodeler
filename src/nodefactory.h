#ifndef starsky__nodefactory_h
#define starsky__nodefactory_h

#include <node.h>
#include <map>

namespace Starsky {

/**
 * Base class for NodeFactory objects.
 * This class creates nodes, or deserializes
 * them from strings.
 *
 */
class NodeFactory {

  public:
    /**
     * This method returns the "next" node.
     * By default, it just returns new Node()
     */
    virtual Node* create() { return newNode(); }
    /**
     * This method deserializes a node.
     * By default it may also keep a table
     * of string -> Node* mappings so that
     * that repeated calls of create(string)
     * will return the same pointer for
     * the same string
     */
    virtual Node* create(const std::string& node);

    /**
     * @param node String representation of the node.
     * If we have not created this node yet, this function
     * returns 0
     */
    virtual Node* getNode(const std::string& node);
    
  protected:
    /**
     * Return the same as new Node() for
     * whatever derived type we have of Node
     */
    virtual Node* newNode() { return new Node(); }
    virtual Node* newNode(const std::string& node) { return new Node(node); }
    
    std::map<std::string, Node*> _str_to_node;
};

}

#endif
