#include "walkandsitepercmessage.h"

using namespace Starsky;
using namespace std;

WalkAndSitePercMessage::WalkAndSitePercMessage(Random& r, double p, int walkttl, int percttl) :
   SitePercMessage(r,p,percttl), _ac_mes(r,1,walkttl), _rand(r), _prob(p) {

}
/**
 * this algorithm is based on the breadth first search Starsky::Network::getDistance.
 */
void WalkAndSitePercMessage::visit(Node* n, Network& net) {

    _ac_mes.forgetVisitedNodes();
    _ac_mes.visit(n,net);
    _crossed_edges += _ac_mes.getCrossedEdgeCount();
    set<Node*>::const_iterator v_it;
    for(v_it = _ac_mes.getVisitedNodes().begin(); v_it != _ac_mes.getVisitedNodes().end(); v_it++) {
        SitePercMessage::visit(*v_it,net);
    }
}

