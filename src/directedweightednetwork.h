#ifndef starsky__directedweightednetwork_h
#define starsky__directedweightednetwork_h

#include <map>
#include <set>
#include <iostream>

#include "network.h"
#include "directednetwork.h"
#include "node.h"
#include "edge.h"

/**
 * The weight of each edge in the network is defined as the # of times that the edge has been traveled.
 */

namespace Starsky {
	
class DirectedWeightedNetwork : public DirectedNetwork {

    public:
	DirectedWeightedNetwork();
	/**
	 * @param in the input file to construct the directed weighted network.  
	 */
	DirectedWeightedNetwork(std::istream& in);
        /**
	 * @param edge the edge to add to the network.  if the edge is already exist, increment its weight by one
	 * @return true if an edge was added successfully, false otherwise
	 */
	bool add(Edge* aEdge);	

	double getAverageWeight(Node* node) const;

	std::map<int, double> getOutDegtoAverageStrengthMap(const NodePSet& nodes) const;

	std::map<int, double> getOutDegtoAverageStrengthMap() const;
	/**
	 * @param start, end are the two nodes of the directed edge that we are interested in
	 * @return the weight of this directed edge
	 */
	double getDirectedWeight(Node* start, Node* end) const;
	/**
	 * @return for each # of weights, the number of edges which have that weight
	 */
	std::map<double, int> getEdgeWeightDist(const EdgeSet& edges) const;

	std::map<double, int> getEdgeWeightDist() const;
	/**
	 * @param node the node that we are interested in
	 * @return the sum of the weighted of all the in-edges connected to node
	 */
	double getInStrength(Node* node) const;
	/**
	 * @param nodes the set of nodes that we are interested in
	 * @return the correlation coefficient between the degree and average strength
	 * IMPORTANT: all input nodes must have outdegree >= 1
	 */
	virtual double getOutDegAverageStrengthCoefficient(NodePSet& nodes);

        virtual double getOutDegAverageStrengthCoefficient();
	/**
	 * @param nodes the set of nodes that we are interested in
	 * @return the correlation coefficient between the degree and strength
	 * IMPORTANT: all input nodes must have outdegree >= 1
	 */
	virtual double getOutDegStrengthCoefficient(NodePSet& nodes);

        virtual double getOutDegStrengthCoefficient();

	virtual std::map<double, int> getNodeOutStrengthDist(NodePSet& nodes);

	virtual std::map<double, int> getNodeOutStrengthDist();
	/**
	 * @param node the node that we are interested in
	 * @return the sum of the weighted of all the outedges connected to node
	 */
	double getOutStrength(Node* node) const;
	/**
	 * @param m moment we are computing
	 * @return <outstrength^m> for the node_set
	 */
        double getOutStrengthMoment(int m) const;
	/**
	 * @param m moment we are computing
	 * @param nodes set of nodes we are looking at the moment of
	 * @return <outstrength^m> for the node_set
	 */
        double getOutStrengthMoment(int m, const NodePSet& nodes) const;
	/**
	 * @param m moment we are computing
	 * @return <(out_strength/k_out)^m> for the out_node_set
	 */
        double getOutAverageStrengthMoment(int m) const;
	/**
	 * @param m moment we are computing
	 * @param nodes set of nodes we are looking at the moment of
	 * @return <(out_strength/k_out)^m> for the node_set
	 */
        double getOutAverageStrengthMoment(int m, const NodePSet& nodes) const;

	//the below two functions are not too useful!
	void printOutDegStrengthPlot(NodePSet& nodes, std::ostream& out);

	void printOutDegStrengthPlot(std::ostream& out);

	/**
	 * @param out the file to write the nodes that have zero in-degree
	 */
	void printInZeros(std::ostream& out) const;
	/**
	 * @param out the file to write the nodes that have zero out-degree
	 */
	void printOutZeros(std::ostream& out) const;

	void printOutStrengthDist(std::ostream& out) ;

	virtual void printOutDegStrengthMap(std::ostream& out);

	/**
	 * Prints all the edges in the network and the weights associated with each edge.
	 */
	void printWeights(std::ostream& out) const;


};
	
}

#endif


