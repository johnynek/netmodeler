#ifndef npartitenetwork
#define npartitenetwork

#include "network.h"
#include <map>
#include <string>

namespace Starsky {

/**
 * Represents networks where there are more than one type of
 * node.  Uses an int to indentify each type of node.  subclasses
 * should probably have some static const variables to give names
 * to the types used.
 */
	
class NPartiteNetwork : public Network {

    public:

	    /**
	     * Add a node an note the type
	     * @param type the type of node this is.
	     * @param aNode the node to add
	     * @return true if this is a new node, else false
	     */
	    virtual bool add(int type, Node* aNode);
	    /**
	     * @param type the type of node that you want to get the set of
	     * @return reference to the set of nodes of this type
	     */
	    virtual const NodePSet& getNodes(int type) const;
	    virtual const NodePSet& getNodes() const { return node_set; }
	    /**
	     * @param node the node to remove
	     * @return the number of edges removed by this removal
	     */
	    virtual int remove(Node* node); 

	    virtual NPartiteNetwork& operator=(const NPartiteNetwork& net);
	  
	    
    protected:
	    std::map<int, NodePSet> _type_to_node_map;
};
	
}
#endif
