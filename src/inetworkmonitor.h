#ifndef starsky__inetworkmonitor_h
#define starsky__inetworkmonitor_h

namespace Starsky {

/* To break any circular dependencies we declare
 * the following:
 */
class Node;
class Edge;
class Network;
	
/*
 Monitor when things happen to networks.
 All changes will be announced
 to the INetworkMonitors which are listening.
 The Network will have a vector<INetworkMonitor*>
 which it will iterate through when there is a change
 */

class INetworkMonitor {

  public:
    /**
     * Called after the monitor is added to the network
     */
    virtual void postAdd(Network* net) { };
    virtual void preNodeAdd(Node* newnode) { };
    virtual void postNodeAdd(Node* newnode, bool success) { };
    virtual void preNodeRemove(Node* node) { };
    virtual void postNodeRemove(Node* node, bool success) { };
    virtual void preEdgeAdd(Edge* newedge) { };
    virtual void postEdgeAdd(Edge* newedge, bool success) { };
    virtual void preEdgeRemove(Edge* edge) { };
    virtual void postEdgeRemove(Edge* edge, bool success) { };
};

}

#endif
