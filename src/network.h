#ifndef starsky__network_h
#define starsky__network_h



#include "node.h"
#include "edge.h"
#include "superstring.h"
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <iostream>
#include <cmath>
#include <list>

namespace Starsky {

/**
 * This handles all the general algorithms on undirected graphs.
 * Subclass this class to make specific types of networks (such as Erdos-Renyi random graphs).
 * IMPORTANT: if you subclass Network to use subclasses of Edge rather than Edge itself, you
 * MUST implement getEdgePtr for this new network, otherwise, things will not work.
 * Note that all memory (Nodes and Edges) is managed by a reference counting system internal
 * to Network.  NEVER delete a Node or Edge outside of network.  When adding Nodes, you
 * could do so using "new Node".
 * As a convenience, you can add edges directly, this makes a copy internally *IF* this is
 * indeed a new edge (using getEdgePtr to check).
 */
class Network {

    public:
	Network();
	Network(const Network& net);
	/**
	 * @param in a stream which contains the network
	 */
	Network(std::istream& in);
	virtual ~Network();

  /*
   * I usually avoid typedefs, but this seemed like a case for it.
   * In some cases, we may want to allow networks with more than one
   * edge between nodes, in other cases not.  By changing this typedef
   * from multiset to set, everything else should work.
   */
	///only to make life easy
        typedef std::set<Node*> NodePSet;	
        //typedef std::multiset<Node*> ConnectedNodePSet;
        //typedef std::multiset<Edge> EdgeSet;
	//The below makes sure that each edge appears only once
	///only to make life easy
	typedef std::set<Node*> ConnectedNodePSet;
	///only to make life easy
	typedef std::set<Edge*> EdgeSet;
	
	/**
	 * @param edge an edge to add.  This just allocates memory for a
	 * copy and calls the above.
	 * @return false if the edge is already in the network, else true
	 */
	virtual bool add(const Edge& edge);
	/**
	 * You should not use this unless you know what you are doing.
	 * The above function is probably what you want.
	 * @param edge a pointer to an edge.  ALWAYS call with syntax: "new Edge()".
	 * memory is managed by Network.
	 * @return false if the edge is already in the network, else true
	 */
        virtual bool add(Edge* edge);
	/**
	 * @return false if the Node* is already in the network, else true
	 */
	virtual bool add(Node* node);
	/**
	 * Empties the network and deletes all the Node*
	 */
	virtual void clear();
        /**
	 * This measures the correlation of degrees at either ends of edges.
	 * @see cond-mat/0205405
	 * @return the assortativity of the network (1.0 to -1.0)
	 */
	double getAssortativity() const;
	/**
	 * @param aNode the node to compute the Associated number of.
	 * @return maximum distance of from aNode to any other Node in the network. (-1 == infinity)
	 */
	int getAssociatedNumber(Node* aNode) const;
	/**
	 * @returns number of nodes at each associated number for all valid values in the network
	 */
	std::map<int, int> getAssociatedNumberDist() const;
	/**
	 * @return average degree of the all nodes in the network
	 */
	double getAverageDegree() const;
	/**
	 * @param nodes a subset of the network to consider
	 * @return the average over this subset
	 */
	double getAverageDegree(const NodePSet& nodes) const;
	/**
	 * @param bin_count the number of evenly spaced bins
	 * @return a histogram evenly spaced over clustering coefficients
	 */
	std::vector<int> getCCHist(int bin_count=10) const;
	
	///get the Average Cluster Coefficient as a function of Degree
	std::map<int, double> getCCvsDegree() const;
	/**
	 * If node1 and node2 are elements of a connected component G, 
	 * then getMinCut returns the set of edges that disconnects G into
	 * G1 and G2, which contain node1 and node2 respectively.  The cutset
	 * must be chosen such that the sum the weights of the edges in the 
	 * cutset are minimal. 
	 * @returns a minimal-weight cutset between node1 and node2.  
	 */
	const EdgeSet& getMinCut(Node* node1,Node* node2) const;
	/**
	 * @param node the node to compute the cluster coefficient for.
	 * @return the clustering coefficient (1.0 to 0.0).
	 */
	virtual double getClusterCoefficient(Node* node) const;
	/**
	 * @param nodes set of nodes to average over
	 * @return the average cluster coefficient of these nodes
	 */
	double getClusterCoefficient(const NodePSet& nodes) const;
	/**
	 * Estimate the variance in the CC using the jackknife
	 * delete-1 method
	 */
	double getCCStdErr() const;
	/**
	 * @return average clustering coefficient for the whole network
	 */
	double getClusterCoefficient() const;
#if 0
	/**
	 * Use the Newman community finding algorithm to find
	 * communities: @see cond-mat/0309508
	 * @param q the Q parameter at each step
	 * @param joins the pair of communities joined at each step
	 * @return the step where the maximum Q is found
	 * The joins are made from initially labelling each node with
	 * an integer (in the node_set order).
	 *
	 * The "join" number refers to the index of the node in the
	 * default ordering of the node_set.  You can get a vector
	 * which has the right order by doing:
	 * vector<Node*> my_vec;
	 * my_vec.insert(my_vec.begin(), node_set.begin(), node_set.end());
	 * 
	 * you can also use getCommunity to get the network that has
	 * all edges BETWEEN communities removed, leaving only the
	 * edges inside communities
	 */
	virtual int getCommunities(std::vector<double>& q,
			   std::vector< std::pair<int,int> >& joins) const;
	/**
	 * using the above, get the particular break down of the communities:
	 */
	std::set<Network> getCommunity(int step,
			    const std::vector< std::pair<int, int> >& joins) const;
	/**
	 * @return a set of networks.  One network for each component.
	 */
	std::set<Network> getComponents() const;
#endif
	/**
	 * @return degree of a given node
	 */
	int getDegree(Node* node) const;
	/**
	 * @return for each degree, the number of nodes which have that degree
	 */
	std::map<int, int> getDegreeDist() const;
	/**
	 * @param nodes the set of nodes to look at
	 * @return for each degree, the number of nodes which have that degree
	 */
	std::map<int, int> getDegreeDist(const NodePSet& nodes) const;

	/**
	 * @return the entropy of the degree distribution
	 */
	double getDegreeEntropy() const;

	/**
	 * @param m moment we are computing
	 * @param nodes set of nodes we are looking at the moment of
	 * @return <k^m> for the set of node_set
	 */
        double getDegreeMoment(int m, const NodePSet& nodes) const;
	/**
	 * @param m moment we are computing
	 * @return <k^m> for the set of node_set
	 */
        double getDegreeMoment(int m) const;

	/**
	 * This function is the underlying function for all the
	 * distance functions.  It does a breadth first search,
	 * and returns the distance for each node, and the set
	 * of leaf nodes (nodes which have no children in the breadth
	 * first tree).
	 * This is the main breadth first search algorithm in network
	 * all others use this one
	 * O(N) memory, O(N) time.
	 * @param start the node to compute all distances from.
	 * @param distances the distance to each node in the graph
	 * @param weights the number of shortest paths from start to this node
	 * @param leaf_nodes the child-less nodes in the breadth-first tree
	 * @return the maximum distance
	 */
	int getDistancesFrom(Node* start,
			 std::map<Node*, int>& distances,
			 std::map<Node*, int>& weights,
			 std::set<Node*>& leaf_nodes,
			 int max_dist = -1) const;
	/**
	 * just a wrapper for getDistancesFrom 
	 * @param start the node to start from
	 * @param end if end is 0, we mean the distance to the furthest node
	 *            in the same component with start
	 */
	int getDistance(Node* start, Node* end=0) const;
	/**
	 * This is O(N) memory, O(N) time.
	 * @deprecated use the one below
	 * @param node the node to see how many neighbors it has at each distance
	 * @param max_dist the maximum distance to go to.  -1 means infinite.
	 * @return a vector<int> which has the number of nodes at [distance]
	 */
	std::vector<int> getDistanceDist(Node* node, int max_dist = -1) const;

	/**
	 * Calls getDistancesFrom to add distances to the distribution
	 * This function does not clear distribution so you can call it on
	 * each node to get the distribution of a set of nodes, or a network.
	 * @param start the node to start from,
	 * @param distribution the distribution to UPDATE.
	 * @return the maximum distance.
	 */
	int getDistanceDist(Node* start,
			    std::map<int, int>& distribution) const;
	/**
	 * Calls getDistancesFrom to add distances to the distribution
	 * This function does not clear distribution so you can call it on
	 * each node to get the distribution of a set of nodes, or a network.
	 * @param distribution the distribution to UPDATE.
	 * @return the radius of the graph.
	 */
	int getDistanceDist(std::map<int, int>& distribution) const;
	
	/**
	 * @deprecated, use the one above
	 * Calls the above function N times.
	 * O(N^2) time, O(N) memory where N is the number of nodes
	 * @return the number of shortest paths which have each given distance
	 */
	std::map<int, int> getDistanceDist() const;
	/**
	 * O(N^2) time O(N) memory where N is the number of nodes
	 * @param nodes the set of nodes you are interesting the distances between
	 * @return the number of shortest paths which have each given distance
	 */
	std::map<int, int> getDistanceDist(const NodePSet& nodes) const;
	/**
	 * @param target the node which we are generating the betweennesses for
	 * @param betweeness the value for of the betweenness from this node (we assume
	 * the input is initialized)
	 * @return the edge with the greatest betweenness.
	 */
	Edge* getEdgeBetweennessFor(Node* target,
			            std::map<Edge*, double>& betweenness) const;
	/**
	 * @param between the betweeness values for all the edges in the graph
	 * @return the edge with the greatest value.
	 * This function basically calls the getEDgeBetweennessFor on each Node in the network
	 */
	Edge* getEdgeBetweenness(std::map<Edge*, double>& betweenness) const;
	/**
	 * when calling getEdges() or getEdges(0) return the set of all edges.
	 * @return the edges that a given Node is an endpoint of
	 */
	virtual const EdgeSet& getEdges(Node* node) const;
	virtual const EdgeSet& getEdges() const;

	/**
	 * @return a map such that result[pair<int,int>(i,j)] = number of edges which
	 * go from degree i to degree j plus the number that go from j to i
	 */
	virtual std::map< std::pair<int,int>,  int > getEdgeDist() const;
	
	/**
	 * @return a pair with the first being entropy of degree of
	 * one side of a node.  the second being the entropy of both
	 * sides of the node.
	 */
	virtual std::pair<double, double> getEdgeEntropy() const;
	/**
	 * @return how much mutual information the degree of a node at one
	 * end of an edge has about another
	 */
        virtual double getEdgeMutualInfo() const;
	
	/**
	 * @param edge The edge to check for a ptr to it in this network
	 * @return 0 if this edge is not in the network, a ptr to it if it is
	 */
	virtual Edge* getEdgePtr(const Edge& edge) const;
	/**
	 * Get the expected transitivity in the random model,
	 * given the degree distribution
	 */
	virtual double getExpectedTransitivity() const;
	/**
	 * @return all neighbors of a given node
	 */
	const ConnectedNodePSet& getNeighbors(Node* node) const;
	/**
	 * @return the nodes whose distance is <= d)
	 */
	NodePSet getNeighborhood(Node* node, int distance) const;
	/**
	 * count the number of nth neighbors for each node, and see how often
	 * each count appears.
	 * @param nth the distance to count. 1 means first neighbors, 2 seccond neighbors...
	 * @return key is the number of neighbors, value is the number of nodes that have that number.
	 */
	virtual std::map<int, int> getNthNeighborDist(int nth) const;
	/**
	 * same as above except with a given set of nodes to consider.
	 */
	virtual std::map<int, int> getNthNeighborDist(const NodePSet& nodes, int nth) const;
	/**
	 * @return a copy of the set<Node*> of all nodes.  Notice,
	 * that if these Nodes might point no
	 * where if the Network goes out of scope (since Networks
	 * do reference counting).
	 */
	virtual const NodePSet& getNodes() const;
	/**
	 * Get a subgraph of this graph using only the given nodes.
	 * The edges in the returned graph are all the edges
	 * that exist between the given nodes
	 * The caller must delete the returned network when done.
	 * @param nodes the nodes for which to build the subgraph
	 * @return the Network which is the subgraph
	 */
	virtual Network* getSubNet(const NodePSet& nodes) const;
	/**
	 * This is an alternative definition of clustering
	 * for the entire graph.  It is:
	 * 3 * number of triangles/(number of wedges)
	 */
	virtual double getTransitivity() const;

	/**
	 * Returns the number of triangles and wedges in the graph
	 * @param triangles pass-by-reference count of the number of triangles
	 * @param wedges pass-by-reference count of the number of wedges
	 */
	virtual void getTrianglesWedges(int& triangles, int& wedges) const;
	/**
	 * Return the number of Wedges in the network.
	 * A wedge is a pair of edges that share exactly
	 * one node
	 */
	virtual int getWedgeCount() const;
	/**
	 * @return true if the network has this edge
	 */
	virtual bool has(const Edge& edge) const;
	/**
	 * @return true if this node is in the network
	 */
	bool has(Node* node) const;
	/**
	 * Prints the network out to graphviz (for now) format.
	 */
	virtual void printTo(std::ostream& out) const;
	/**
	 * Prints the network out to GDL format.  See http://www.absint.com/aisee/manual/windows/node58.html
	 */
  virtual void printToGDL(std::ostream& out) const;
	/**
	 * Remove the node and connect all its neighbors
	 * to one another.
	 * @param node to remove
	 * @return number of neighbors the node had
	 */
	virtual int removeAndCluster(Node* node);
	virtual int removeAndCluster(const NodePSet& node);
	/**
	 * @return the number of edges actually removed
	 * (0 means the edge is not in the network).
	 */
	virtual int remove(const Edge& edge);
	virtual int remove(Edge* e);
	/**
	 * @return the number of edges removed
	 */
	virtual int remove(Node* node);
	/**
	 * @param aset a set of nodes to remove from the network
	 * @return the number of edges removed by this operation
	 */
	virtual int remove(const NodePSet& aset);
        /**
	 * the STL uses < to sort inside of sets, we return set<Network>,
	 * so we need this operator
	 */
	virtual bool operator<(const Network& aNet) const;
	/**
	 * copies all members, and increments the
	 * reference counts for Node*s and Edge*s 
	 */
	virtual Network& operator=(const Network& aNet);
	virtual	bool Network::Merge(const Network& aNet, int aStart);

    protected:
	/**
	 * This is used by the constructor which
	 * takes the istream input.  It should
	 * not be used on existing networks, only
	 * when constructing new ones.
	 */
	virtual void readFrom(std::istream& in);

  virtual void readFromGDL(std::istream& in);
	    /**
	     * Just a set which holds pointers to all nodes in the network
	     */
	    NodePSet node_set;
	    /**
	     * set of all the edges in the network.
	     */
	    EdgeSet edge_set;
	    /**
	     * This holds the structure of the graph.  For each Node*, we get the set
	     * of Node* that it is connected to.
	     */
	    std::map<Node*, ConnectedNodePSet > connection_map;
            /**
	     * Holds the edges each node is involved with:
	     */
	    std::map<Node*, EdgeSet> _node_to_edges;
	    /**
	     * sometimes we need to return a const reference to an empty node set
	     */
	    static const ConnectedNodePSet _empty_cnodeset;
	    static const NodePSet _empty_nodeset;
	    static const EdgeSet _empty_edgeset;
	    /**
	     * reference counting for Nodes and Edges.  This is done
	     * when we copy stuff around.
	     */
	    int incrementEdgeRefCount(Edge* edge);
	    /**
	     * deletes the memory for edge when there are no references to it.
	     */
	    int decrementEdgeRefCount(Edge* edge);
	    int incrementNodeRefCount(Node* node);
	    /**
	     * delete the memory for node when there are no references to it.
	     */
	    int decrementNodeRefCount(Node* node);
	    static std::map<Node*, int> _node_ref_count;
	    static std::map<Edge*, int> _edge_ref_count;
  };

}
#endif

