#ifndef starsky__spatialnode
#define starsky__spatialnode

#include "node.h"
#include "random.h"
#include <vector>
#include <cmath>

namespace Starsky {

 /**
  * represents a node which has a position in a space.
  * We will consider a hypercube with each side of length 1.
  */
	
class SpatialNode : public Node {

    public:
        SpatialNode(Random& rand, int dim);
	SpatialNode(const std::vector<double>& pos);
	
	const std::vector<double>& getPosition() const;
        double getDistanceTo(SpatialNode* aNode) const;
	
    protected:
        std::vector<double> _coords;

	
};
	
}

#endif
