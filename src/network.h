/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef starsky__network_h
#define starsky__network_h

#define HIDE_STL

#include "node.h"
#include "edge.h"
#include "iterator.h"
#include "inetworkmonitor.h"
#include "superstring.h"
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <iostream>
#include <cmath>
#include <list>
#include <algorithm>

namespace Starsky {
	
/**
 * This handles all the general algorithms on undirected graphs.
 * Subclass this class to make specific types of networks.
 *
 * Also note that in this codebase, Networks are used as general
 * storage containers for Node and Edge objects.  In those cases,
 * you can think of the Network as being used as a set of Nodes,
 * a set of Edges, or perhaps a mapping of nodes.
 * 
 * IMPORTANT: if you subclass Network to use subclasses of Edge rather than Edge itself, you
 * MUST implement getEdgePtr for this new network, otherwise, things will not work.
 * Note that all memory (Nodes and Edges) is managed by a reference counting system internal
 * to Network.  NEVER delete a Node or Edge outside of network.  When adding Nodes, you
 * could do so using "new Node".
 * As a convenience, you can add edges directly, this makes a copy internally *IF* this is
 * indeed a new edge (using getEdgePtr to check).
 */

/**
 * The NodeIterator is used a lot by this class, so we typedef it here
 */
typedef Iterator<Node> NodeIterator;
/**
 * The EdgeIterator is used a lot by this class, so we typedef it here
 */
typedef Iterator<Edge> EdgeIterator;

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
	 * Add all the nodes and edges of a given network to this one
	 */
	virtual void add(Network* n);
	/**
	 * @return false if the Node* is already in the network, else true
	 */
	virtual bool add(Node* node);
	/**
	 * @param INetworkMonitor to add:
	 */
	virtual void add(INetworkMonitor* nm);
	/**
	 * Empties the network and deletes all the Node*
	 */
	virtual void clear();

	/**
	 * Removes all edges from the network, but leaves the nodes
	 */
	virtual void clearEdges();
#ifndef HIDE_STL
	/**
	 * Use STL algorithm for_each on all the nodes
	 */
	template <class T>
	T forEachNode(T fun)
	{
          return for_each(node_set.begin(), node_set.end(), fun);
        }
	/**
	 * Use STL algorithm for_each on all the edges 
	 */
	template <class T>
	T forEachEdge(T fun)
	{
          return for_each(edge_set.begin(), edge_set.end(), fun);
        }
#endif
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
	 * @param nodes a subset of nodes to consider
	 * @return the average over this subset
	 */
	double getAverageDegree(NodeIterator* nodes) const;
	/**
	 * @param bin_count the number of evenly spaced bins
	 * @return a histogram evenly spaced over clustering coefficients
	 */
	std::vector<int> getCCHist(int bin_count=10) const;
	
	///get the Average Cluster Coefficient as a function of Degree
	std::map<int, double> getCCvsDegree() const;
	/**
	 * @param node the node to compute the cluster coefficient for.
	 * @return the clustering coefficient (1.0 to 0.0).
	 */
	virtual double getClusterCoefficient(Node* node) const;
	/**
	 * @param nodes set of nodes to average over
	 * @return the average cluster coefficient of these nodes
	 */
	double getClusterCoefficient(NodeIterator* nodes) const;
	/**
	 * Estimate the variance in the CC using the jackknife
	 * delete-1 method
	 */
	double getCCStdErr() const;
	/**
	 * @return average clustering coefficient for the whole network
	 */
	double getClusterCoefficient() const;
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
	std::map<int, int> getDegreeDist(NodeIterator* nodes) const;

	/**
	 * @return the entropy of the degree distribution
	 */
	double getDegreeEntropy() const;

	/**
	 * @param m moment we are computing
	 * @param nodes set of nodes we are looking at the moment of
	 * @return <k^m> for the set of node_set
	 */
        double getDegreeMoment(int m, NodeIterator* nodes) const;
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
         * @return a network that contains the edges in this network which
         * have one end with node
         * This is the star network (tree) with node in the center
         */
        virtual Network* getEdges(Node* node) const;
        /**
         * @param from the node the edge starts at
         * @param to the node the edge ends at
         * @return the edge that goes between these nodes, null if no edge.
         */
        virtual Edge* getEdge(Node* from, Node* to) const;

	/**
	 * @return an iterator to loop through the edges with
	 */
	virtual EdgeIterator* getEdgeIterator() const;
	
	/**
	 * @return an iterator to all edges connected to a given node
	 */
	virtual EdgeIterator* getEdgeIterator(Node* n) const;
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
	 * @return the number of edges in the network
	 */
	int getEdgeSize() const;
	/**
	 * Get the expected transitivity in the random model,
	 * given the degree distribution
	 */
	virtual double getExpectedTransitivity() const;
	/**
	 * @return a node iterator that iterates over a node's neighbors
	 */
	virtual NodeIterator* getNeighborIterator(Node* node) const;
        /**
         * @return a network which only contains the nodes
         * which are neighbors of the given node, and no edges
         */
        virtual Network* getNeighbors(Node* node) const;
        /**
         * Get the neighborhood of a node, all nodes, and edges between
         * those nodes.
         * This includes the node argument:
         */
        virtual Network* getNeighborhood(Node* node) const;

#ifndef HIDE_STL
	/**
	 * @return the nodes whose distance is <= d)
	 */
	NodePSet getNeighborhood(Node* node, int distance) const;
#endif
	/**
	 * Return a NodeIterator that can iterator through all the nodes
	 */
	virtual NodeIterator* getNodeIterator() const;
	/**
	 * @return the number of nodes
	 */
	int getNodeSize() const;
	/**
	 * Get a subgraph of this graph using only the given nodes.
	 * The edges in the returned graph are all the edges
	 * that exist between the given nodes
	 * The caller must delete the returned network when done.
	 * @param nodes the nodes for which to build the subgraph
	 * @return the Network which is the subgraph
	 */
	virtual Network* getSubNet(NodeIterator* nodes) const;
	/**
	 * This is an alternative definition of clustering
	 * for the entire graph.  It is:
	 * 3 * number of triangles/(number of wedges)
	 */
	virtual double getTransitivity() const;

	/**
	 * @return the number of triangle that involve this node
	 */
	virtual int getTriangles(Node* n) const;

	/**
	 * @return the number of triangles that contain this edge
	 */
	virtual int getTriangles(Edge* e) const;
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
	 * @return true if the network has an equivalent edge
	 */
	virtual bool has(const Edge& edge) const;
        /**
         * Note that has(e) and has(&e) might not return the same thing,
         * one asks if a particular pointer is in the network, the other
         * asks if an EQUIVALENT edge is in the network.
         *
         * @return true if the network has this exact pointer to an edge
         */
        virtual bool has(Edge* edge) const;
	/**
	 * @return true if this node is in the network
	 */
	bool has(Node* node) const;
	/**
	 * @return a new network of the same type as the one called on
	 */
	virtual Network* newNetwork() const;
	/**
	 * Prints the network out to graphviz (for now) format.
	 */
	virtual void printTo(std::ostream& out) const;
	/**
	 * Prints the network out to GDL format.
	 * @see http://www.absint.com/aisee/manual/windows/node58.html
	 */
  virtual void printToGDL(std::ostream& out) const;
	/**
	 * Remove the node and connect all its neighbors
	 * to one another.
	 * @param node to remove
	 * @return number of neighbors the node had
	 */
	virtual int removeAndCluster(Node* node);
	/**
	 * @return the number of edges actually removed
	 * (0 means the edge is not in the network).
	 */
	virtual int remove(const Edge& edge);
	virtual int remove(Edge* e);
	/**
	 * Remove all the edges specified by this iterator
	 * NEVER remove using an iterator and then call moveNext().
	 * This function does the right thing (copy the current,
	 * moveNext, then remove).
	 * @param edges the iterator for the edges to remove
	 */
	virtual int remove(EdgeIterator* edges);
	/**
	 * @return the number of edges removed
	 */
	virtual int remove(Node* node);
	/**
	 * NEVER remove using an iterator and then call moveNext().
	 * This function does the right thing (copy the current,
	 * moveNext, then remove).
	 * @param aset a set of nodes to remove from the network
	 * @return the number of edges removed by this operation
	 */
	virtual int remove(NodeIterator* aset);
	
	virtual void remove(INetworkMonitor* nm);
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
#if 0
	virtual	bool Network::Merge(const Network& aNet, int aStart);
#endif

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
	     * Holds all the network monitors:
	     */
            std::list<INetworkMonitor*> _net_mon;
	    /**
	     * sometimes we need to return a const reference to an empty node set
	     */
	    static const NodePSet _empty_nodeset;
	    static const EdgeSet _empty_edgeset;
	    static const Network _empty_net;
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

	    /**
	     * Here are some inner classes to implement some interators
	     */
	    /**
	     * NodeIterator for the default Network class
	     */
	    class NetNodeIterator : NodeIterator {
	      public: 
		virtual NodeIterator* clone();
                virtual Node* current();
	        virtual bool moveNext();
	        virtual void reset();
		friend class Network;
              protected:
                std::map< Node*, std::set<Edge*> >::const_iterator _nit;
                std::map< Node*, std::set<Edge*> >::const_iterator _begin;
                std::map< Node*, std::set<Edge*> >::const_iterator _end;
                bool _called_movenext;
	    };
            
	    /**
	     * EdgeIterator for the default Network class
	     */
	    class NetEdgeIterator : EdgeIterator {
	      public: 
                virtual EdgeIterator* clone();
                virtual Edge* current();
	        virtual bool moveNext();
	        virtual void reset();
		friend class Network;
              protected:
                std::map< Node*, std::set<Edge*> >::const_iterator _nit;
                std::map< Node*, std::set<Edge*> >::const_iterator _begin;
                std::map< Node*, std::set<Edge*> >::const_iterator _end;
                std::set<Edge*>::const_iterator _eit;
                bool _called_movenext;
	    };
	    
            /**
             * This is the iterator for iterating over neighbors,
             * which requires slightly different internal logic
             */
	    class NeighborIterator : NodeIterator {
	      public: 
		virtual NodeIterator* clone();
                virtual Node* current();
	        virtual bool moveNext();
	        virtual void reset();
		friend class Network;
	      protected:
	        Node* _neighbors_of;  //This is the node that we are looking at neighbors of
		EdgeSet::const_iterator _eit; //Iterator for the edges of this node
		EdgeSet::const_iterator _beg;
		EdgeSet::const_iterator _end;
		bool _moved_to_first; //Start before the first
	    };
	    /**
	     * This allows us ot iterate over the edges connected to a node
	     */
	    class NeighborEdgeIterator : EdgeIterator {
	      public: 
                virtual EdgeIterator* clone();
                virtual Edge* current();
	        virtual bool moveNext();
	        virtual void reset();
		friend class Network;
	      protected:
	        Node* _neighbors_of;  //This is the node that we are looking at neighbors of
		EdgeSet::const_iterator _eit; //Iterator for the edges of this node
		EdgeSet::const_iterator _beg;
		EdgeSet::const_iterator _end;
		bool _moved_to_first; //Start before the first
	    };
	    
	private:
            /**
	     * Here is our current implementation.  Not even subclasses
	     * should assume this is how it works.
	     *
	     * Holds the edges each node is involved with:
	     */
	    std::map<Node*, EdgeSet> _node_to_edges;
	    //This is the total number of edges:
	    int _edge_count;
  };

}
#endif


