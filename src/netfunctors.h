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

#ifndef starsky__netfunctors_h
#define starsky__netfunctors_h

#include <network.h>
#include <cmath>
#include <iostream>
#include <algorithm>

namespace Starsky {

/**
 * This file contains a functors for use with STL 
 * algorithms.
 */

/**
 * Here is a sorting functor for Network* objects
 * Returns true if the first is >= the second
 *
 * Useful for STL algorithms
 */
struct networkptr_gt : public std::binary_function<Network*, Network*, bool> {
  bool operator()(Network* x, Network* y) {
    return !(*x < *y);
  }
};

struct networkptr_lt : public std::binary_function<Network*, Network*, bool> {
  bool operator()(Network* x, Network* y) {
    return (*x < *y);
  }
};

typedef int (Network::*NodeIntMember)(Node*) const;
typedef double (Network::*NodeDoubleMember)(Node*) const;

struct node_i_func : public std::unary_function<Node*, int>
{
  public:
    node_i_func(Network* net, NodeIntMember mem) : sum(0), calls(0),
						   _func(mem), _net(net)
    {
    
    }
    int sum;
    int calls;
    NodeIntMember _func;
    Network* _net;
    
    int operator()(Node* n)
    {
      calls++;
      int val = (_net->*_func)(n);
      sum += val;
      return val;
    }

    double average() { return (double)sum/(double)calls; }
};

struct node_d_func : public std::unary_function<Node*, double>
{
  public:
    node_d_func(Network* net, NodeIntMember mem, double p=1.0) : _power(p),
						   sum(0.0), calls(0),
						   _ifunc(mem), _net(net)
    {
      _dfunc = 0;
    }
    node_d_func(Network* net, NodeDoubleMember mem, double p = 1.0) :
	                                           _power(p),
						   sum(0.0), calls(0),
						   _dfunc(mem), _net(net)
    {
      _ifunc = 0;
    }
    double sum;
    int calls;
    
    double _power;
    NodeIntMember _ifunc;
    NodeDoubleMember _dfunc;
    Network* _net;
    
    double operator()(Node* n)
    {
      calls++;
      double val;
      if( _dfunc == 0 ) {
        val = (double)(_net->*_ifunc)(n);
      }
      else {
        val = (_net->*_dfunc)(n);
      }
      val = pow(val, _power);
      sum += val;
      return val;
    }
    double average() { return (double)sum/(double)calls; }
};


/**
 * A functor that returns the degree of a node,
 */
struct node_deg : public std::unary_function<Node*, int>
{
 public:
  node_deg(Network* my_net) : calls(0), sum(0), net(my_net) { }
  int sum;
  int calls;
  int operator() (Node* n) {
    calls++;
    int deg = net->getDegree(n);
    sum += deg;
    return deg;
  }
  Network* net;
};

/**
 * Returns a power of the node's degree
 */
class node_deg_pow : public std::unary_function<Node*, double>
{
 public:
  node_deg_pow(Network* my_net, double myp) : calls(0), sum(0.0),
					     net(my_net), power(myp) { }
  double sum;
  int calls;
  double operator() (Node* n) {
    calls++;
    double deg = (double)net->getDegree(n);
    double p = pow(deg, power);
    sum += p;
    return deg;
  }
  
  Network* net;
  double power;
};

}
#endif
