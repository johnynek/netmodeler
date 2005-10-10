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

#ifndef starsky__nodeintstats_h
#define starsky__nodeintstats_h

#include <map>
#include <node.h>
#include <network.h>

namespace Starsky {

/**
 * Pointer to member function which takes a node and returns an int
 */
typedef int (Network::*NodeIntMember)(Node*) const;
	
/**
 * This class collects statistics from functions
 * that take nodes and return ints.
 */
	
class NodeIntStats {
  public:
    /**
     * @param keep_dist if true, keep the distribution in a map<int, int>
     * @param keep_max_net if true, keep a Network which has nodes with the max
     * @param keep_min_net if true, keep a Network which has node with the min
     */
    NodeIntStats(bool keep_dist = false, bool keep_max_net = false,
		 bool keep_min_net = false);
    
    ~NodeIntStats();
    /**
     * Collect statistics from the given network
     * @param net the Network to get the values from
     * @param mem the function to look at statistics of
     * @param ni if not zero, iterate over these nodes and delete the iterator
     */
    void collect(const Network* net, NodeIntMember mem, Iterator<Node>* ni = 0);
    /**
     * Collect the join statistics by looking at the values at either
     * end of an edge.  For this to be useful, you must be keeping
     * distributions (see the constructor).  
     */
    void collectByEdge(const Network* net, NodeIntMember mem, Iterator<Edge>* ei=0);
    /**
     * @return the number of nodes we iterated over
     */
    int getCount() const;
    /**
     * This is empty if we are not collecting the distribution
     * @return a map containing the distribution
     */
    const std::map<int, int>& getDist() const;
    /**
     * @return the maximum value found
     */
    int getMax() const;
    /**
     * @return the nodes which have the maximum value
     */
    Network* getMaxNet() const;
    /**
     * @return the minimum value found
     */
    int getMin() const;
    /**
     * @return the nodes which have the minimum value
     */
    Network* getMinNet() const;
    /**
     * @return the sum
     */
    int getSum() const;
    /**
     * @return the squared sum
     */
    int getSquaredSum() const;
    /**
     * @return the average
     */
    double getAverage() const;
    /**
     * @return the shannon entropy of the distribution
     */
    double getEntropy() const;
    /**
     * Must call collectByEdge first
     * @return the correlation coefficient when looking at values across an edge
     */
    double getEdgeCorrelation() const;
    /**
     * Must call collectByEdge first
     * @param hstart the entropy of the value at the start Node
     * @param hend the entropy of the value at the end Node
     * @param hse the joint entropy
     */
    void getEdgeEntropy(double& hstart, double& hend, double& hse) const;

    /**
     * @see NodeIntStats::getEdgeEntropy,
     * @return the mutual information from the start to the end of the node
     */
    double getEdgeMutualInfo() const {
      double h1, h2, h3;
      getEdgeEntropy(h1,h2,h3);
      return h1 + h2 - h3;
    }
    /**
     * @return the second moment
     */
    double getMoment2() const;
    /**
     * If you keep the distribution, you can compute any moment.
     * If you don't keep the distribution, this meaningless
     */
    double getMoment(double m) const;
    /**
     * @return variance
     */
    double getVariance() const;
  protected:
    std::map<int, int> _dist;
    ///Keeps the number of edges which go from valuex to valuey
    std::map< std::pair<int, int>, int> _edge_dist;
    bool _keep_dist;
    bool _keep_max_net;
    bool _keep_min_net;
    int _calls;
    int _sum;
    int _sum2;
    int _max;
    int _min;
    Network* _min_net;
    Network* _max_net;
};
	
}

#endif
