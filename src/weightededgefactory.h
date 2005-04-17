#ifndef starsky__weightededgefactory_h
#define starsky__weightededgefactory_h

#include <edge.h>

namespace Starsky {

/**
 * A templated class to create
 * edges.  Any attribute other
 * than the direction of the edge
 * are passed as a string
 */
class WeightedEdgeFactory : public EdgeFactory {
  public:
    virtual Edge* create(Node* from, Node* to) { return new Edge(from, to); }
    virtual Edge* create(Node* from, Node* to, const std::string& attr) {
      double weight = atof(attr.c_str());
      return new Edge(from, to, weight);
    }
};

}

#endif
