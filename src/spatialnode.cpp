#include "spatialnode.h"

using namespace Starsky;
using namespace std;

SpatialNode::SpatialNode(Random& rand, int dim) {
    _coords.resize(dim);
    
    while(dim-- > 0) {
        _coords.push_back( rand.getDouble01() );
    }
}

SpatialNode::SpatialNode(const vector<double>& pos) : _coords(pos) { }

const vector<double>& SpatialNode::getPosition() const {
  return _coords;
}

double SpatialNode::getDistanceTo(SpatialNode* a_node) const {
	
    const vector<double>& other_pos = a_node->getPosition();
    if( other_pos.size() != _coords.size() ) {
        return -1.0;
    }

    unsigned int i;
    double dist2 = 0.0;
    for( i = 0; i < _coords.size(); i++) {
        dist2 += ( other_pos[i] - _coords[i] ) * ( other_pos[i] - _coords[i] );
    }
    return sqrt( dist2 );
}
