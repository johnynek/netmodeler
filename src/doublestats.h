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

#ifndef starsky__doublestats_h
#define starsky__doublestats_h

#include "network.h"
#include "iterator.h"

namespace Starsky {
	
/**
 * Use this to collect statistics from double returning
 * functions of Network.
 */
class DoubleStats {

  public:
    /**
     * @param keep_max_net set to true if you want to keep a network of the max values
     * @param keep_max_net set to true if you want to keep a network of the min values
     */
    DoubleStats(bool keep_max_net = false, bool keep_min_net = false);
    ~DoubleStats();
    /**
     * collecting for the edges and nodes is identical, use a template
     */
    template<class N, class Argname>
    double collect(const N* net, double (N::*mem)(Argname) const,
	    Iterator<Argname>* ni)
    {
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
        const Argname& this_node = ni->current();
        double val = (net->*mem)(this_node);
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
        _sum += val;
        _sum2 += val*val;
        _calls++;
        first = false;
      }
      //Most common statistic is the average
      return getAverage();
    }
    
    /**
     * @return the average value
     */
    double getAverage() const { return _sum/_calls; }
    /**
     * @return the number of times we called the function we collected
     */
    int getCount() const { return _calls; }
    /**
     * @return the maximum value
     */
    double getMax() const { return _max; }
    /**
     * @return a network of Nodes/Edges that have the maximum value
     */
    Network* getMaxNet() const { return _max_net; }
    /**
     * @return the minimum value
     */
    double getMin() const { return _min; }
    /**
     * @return a Network of the Nodes/Edges that take min value. call after collect
     */
    Network* getMinNet() const { return _min_net; }
    /**
     * @return the second moment. call after collect
     */
    double getMoment2() const { return _sum2/_calls; } 
    /**
     * @return the variance. call after collect
     */
    double getVariance() const {
      double m1 = getAverage();
      double m2 = getMoment2();
      return (m2 - m1*m1);
    }
    
  protected:
    ///Keeps the number of edges which go from valuex to valuey
    bool _keep_max_net;
    bool _keep_min_net;
    int _calls;
    double _sum;
    double _sum2;
    double _max;
    double _min;
    Network* _min_net;
    Network* _max_net;
   
};
	
}

#endif
