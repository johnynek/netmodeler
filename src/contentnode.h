#ifndef starsky__contentnode
#define starsky__contentnode

//#define DEBUG
#include "node.h"

namespace Starsky {

/**
 * In keeping with a general graph theoretic structure, we model content in
 * P2P systems as nodes in a graph.  ContentNodes will usually only be attached
 * to Nodes.
 * @see Starsky::ContentNetwork
 */
	
class ContentNode : public Node {
#ifdef DEBUG
  /* public:
  ContentNode();
  ContentNode(int id_num) {id = id_num;}
  virtual ~ContentNode() {};
  int getID() {return id;}

 protected:
 int id;*/
#endif
};
	
}

#endif
