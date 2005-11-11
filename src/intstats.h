/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
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

#ifndef starsky__intstats_h
#define starsky__intstats_h

#include <map>
#include <node.h>
#include <network.h>

namespace Starsky {

/**
 * This class collects statistics from functions
 * that return ints.
 */
	
class IntStats {
  public:
    /**
     * @param keep_dist if true, keep the distribution in a map<int, int>
     * @param keep_max_net if true, keep a Network which has nodes with the max
     * @param keep_min_net if true, keep a Network which has node with the min
     */
    IntStats(bool keep_dist = true, bool keep_max_net = false,
		 bool keep_min_net = false);
    
    ~IntStats();
    /**
     * Collect statistics from the given network
     * @param net the Network to get the values from
     * @param mem the function to look at statistics of
     * @param ni if not zero, iterate over these nodes
     * @return average, which is a common statistic we want to look at
     */
    template<typename N, typename Argname>
    double collect(const N* net, int (N::*mem)(Argname ) const, Iterator<Argname>* ni)
    {
      _dist.clear();
      _calls = 0;
      _sum = 0;
      _sum2 = 0;
      _max = 0;
      _min = 0;
      if( _keep_max_net ) {
        if( _max_net != 0 ) { delete _max_net; }
        _max_net = net->newNetwork();
      }
      if( _keep_min_net ) {
        if( _min_net != 0 ) { delete _min_net; }
        _max_net = net->newNetwork();
      }
      
      //Now its time to iterate:
      bool first = true;
      while( ni->moveNext() ) {
        Argname this_node = ni->current();
        int val = (net->*mem)(this_node);
        //Check to see if this is the max:
        if( first || ( val > _max ) ) {
          _max = val;
          if( _keep_max_net ) {
            _max_net->clear();
            _max_net->add( this_node );
          }
        }
        //Check to see if this is the min:
        if( first || (val < _min) ) {
          _min = val;
          if( _keep_min_net ) {
            _min_net->clear();
            _min_net->add( this_node );
          }
        }
        if( _keep_dist ) {
          _dist[val] = _dist[val] + 1;
        }
        _sum += val;
        _sum2 += val*val;
        _calls++;
        first = false;
      }
      return getAverage();  
    }
    /**
     * Overloaded method so we don't have to make a NodeIterator
     * in the most common case
     */
    double collect(const Network* net, NodeIntMember mem) {
      std::auto_ptr<NodeIterator> ni( net->getNodeIterator() );
      return collect(net, mem, ni.get() );
    }
    /**
     * This is exactly like the above, but instead of calling the function
     * by using C++ syntax, you use "methodname".  This is a hack to make
     * wrapping the library easier from other languages.  Note, function
     * has to be in the table this class keeps.
     */
    double collectN(const Network* net, std::string methodname, NodeIterator* ni = 0);
    /**
     * Overloaded method so we don't have to make a NodeIterator
     * in the most common case
     */
    double collect(const Network* net, EdgeIntMember mem) {
      std::auto_ptr<EdgeIterator> ni( net->getEdgeIterator() );
      return collect(net, mem, ni.get() );
    }
    /**
     * Collect the joint statistics by looking at the values at either
     * end of an edge.  For this to be useful, you must be keeping
     * distributions (see the constructor).  
     */
    void collectByEdge(const Network* net, NodeIntMember mem, EdgeIterator* ei=0);
    /**
     * Same as the above but we use a string to get the methodname.
     * Here to make wrapping easier
     */
    void collectByEdge(const Network* net, std::string methodname, EdgeIterator* ei=0);
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
    //Here is a map of the functions:
    std::map<std::string, NodeIntMember> _nmems;
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
