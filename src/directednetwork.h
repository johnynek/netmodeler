#ifndef starsky__directednetwork_h
#define starsky__directednetwork_h

#include <map>
#include <set>
#include <iostream>
#include <math.h>
//Need math.h for a sqrt

#include "network.h"
#include "node.h"
#include "edge.h"
#include "directededge.h"

namespace Starsky {
	
class DirectedNetwork : public Network {

    public:
	DirectedNetwork();
	DirectedNetwork(std::istream& in);
	bool add(const Edge& e);
	double getAssortativity() const;
	const EdgeSet& getEdges() const;
	Edge* getEdgePtr(const Edge& aEdge) const;
	std::map<int, int> getInDegreeDist(const NodePSet& nodes) const;
	std::map<int, int> getInDegreeDist() const;
	int getInDegree(Node* node) const;
	std::map<int, int> getOutDegreeDist(const NodePSet& nodes) const;
	std::map<int, int> getOutDegreeDist() const;
	int getOutDegree(Node* node) const;
	int getOutSize(){ return out_node_set.size();}  
	void moveIntoUndirectedNetwork(Network& net);
	void printEdges(std::ostream& out) const;	
	void printTo(std::ostream& out) const;
	int remove(const Edge& edge);
	int remove(Edge* edge);
	int remove(Node* node);
	//Reverse ALL the edge directions
	void reverseEdges();
	
	/**
	 * @param m moment we are computing
	 * @param nodes set of nodes we are looking at the moment of
	 * @return <k_in^m> for the set of node_set
	 */
  double getInDegreeMoment(int m, const NodePSet& nodes) const;
	/**
	 * @param m moment we are computing
	 * @return <k_in^m> for the set of node_set
	 */
  double getInDegreeMoment(int m) const;
	/**
	 * @param m moment we are computing
	 * @param nodes set of nodes we are looking at the moment of
	 * @return <k_out^m> for the set of node_set
	 */
  double getOutDegreeMoment(int m, const NodePSet& nodes) const;
	/**
	 * @param m moment we are computing
	 * @return <k_out^m> for the set of node_set
	 */
  double getOutDegreeMoment(int m) const;
				
	/**
	 * @return all nodes that share a directededge that terminates at
	 * the given(argument) node.
	 */
	const ConnectedNodePSet& getInNeighbors(Node* node) const;
	
	//get the number of nodes that can communicate to arg node
	int getInComponentSize(Node* node) const;
	
	/**
	 * @return all nodes that share a directededge that orginates at
	 * the given(argument) node.
	 */
	const ConnectedNodePSet& getOutNeighbors(Node* node) const;

	std::set<DirectedNetwork> getUndirectedComponents() const;

	/**
	 * @param in the input file stream, must be in the format start node : end node
	 */
	void readFrom(std::istream& in);

	/**
	 * @param nodes the set of nodes that we are considering
	 * this function sets the out_node_set to contain only the nodes with degree >=1
	 */
	void setOutNodes(NodePSet& nodes);
	/**
	 * this function sets the out_node_set to contain only the nodes with degree >=1 from node_set
	 */
	void setOutNodes();

 protected:
	NodePSet out_node_set;

	virtual bool add(Edge* aEdge);
	//connection_map which is inherited from Network
	//is used to store the out degrees.  The below is
	//used to store in degrees.
	std::map<Node*, ConnectedNodePSet > in_connection_map;
};
	
}

#endif


