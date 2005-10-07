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
	virtual bool add(const Edge& e);
	virtual bool add(Edge* aEdge);
	double getAssortativity() const;
	Edge* getEdgePtr(const Edge& aEdge) const;
	std::map<int, int> getInDegreeDist(NodeIterator* nodes) const;
	std::map<int, int> getInDegreeDist() const;
	int getInDegree(Node* node) const;
	std::map<int, int> getOutDegreeDist(NodeIterator* nodes) const;
	std::map<int, int> getOutDegreeDist() const;
	int getOutDegree(Node* node) const;
	int getOutSize(){ return out_node_set.size();}  
	void moveIntoUndirectedNetwork(Network& net);
	virtual DirectedNetwork* newNetwork() const { return new DirectedNetwork(); }
	void printTo(std::ostream& out) const;
	//Reverse ALL the edge directions
	void reverseEdges();
	
	/**
	 * @param m moment we are computing
	 * @param nodes set of nodes we are looking at the moment of
	 * @return <k_in^m> for the set of node_set
	 */
  double getInDegreeMoment(int m, NodeIterator* nodes) const;
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
  double getOutDegreeMoment(int m, NodeIterator* nodes) const;
	/**
	 * @param m moment we are computing
	 * @return <k_out^m> for the set of node_set
	 */
  double getOutDegreeMoment(int m) const;
				
#ifndef HIDE_STL
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
#endif
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

};
	
}

#endif


