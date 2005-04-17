#ifndef starsky__edgefactory_h
#define starsky__edgefactory_h

#include <edge.h>

namespace Starsky {

/**
 * A templated class to create
 * edges.  Any attribute other
 * than the direction of the edge
 * are passed as a string
 */
class EdgeFactory {
  public:
    virtual Edge* create(Node* from, Node* to) { return new Edge(from, to); }
    /**
     * The default implemenation returns the same as the above
     */
    virtual Edge* create(Node* from, Node* to, const std::string& attr) {
      return new Edge(from, to);
    }
};
	
}

#endif
