#include "walkandpercmessage.h"

using namespace Starsky;
using namespace std;

WalkAndPercMessage::WalkAndPercMessage(Random& r, double p, int walkttl, int percttl) :
   PercolationMessage(r,p,percttl), _ac_mes(r,1,walkttl), _rand(r), _prob(p) {

}
/**
 * this algorithm is based on the breadth first search Starsky::Network::getDistance.
 */
void WalkAndPercMessage::visit(Node* n, Network& net) {

    _ac_mes.forgetVisitedNodes();
    _ac_mes.visit(n,net);
    _crossed_edges += _ac_mes.getCrossedEdgeCount();
    set<Node*>::const_iterator v_it;
    for(v_it = _ac_mes.getVisitedNodes().begin(); v_it != _ac_mes.getVisitedNodes().end(); v_it++) {
        PercolationMessage::visit(*v_it,net);
    }
}
