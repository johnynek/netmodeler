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

#include "intstats.h"

using namespace Starsky;

IntStats::IntStats(bool keep_dist,
			   bool keep_max_net,
		           bool keep_min_net) : _keep_dist(keep_dist),
                                                _keep_max_net(keep_max_net),
                                                _keep_min_net(keep_min_net)
{
  _min_net = 0;
  _max_net = 0;
}

IntStats::~IntStats()
{
  if( _min_net != 0 ) { delete _min_net; }
  if( _max_net != 0 ) { delete _max_net; }
}

void IntStats::collectByEdge(const Network* net, NodeIntMember mem, EdgeIterator* ei)
{
  bool del_ei = false;
  _edge_dist.clear();
  if( ei == 0 ) {
    ei = net->getEdgeIterator();
    del_ei = true;
  }
  while( ei->moveNext() ) {
    Edge* this_edge = ei->current();
    Node* n1 = this_edge->first;
    Node* n2 = this_edge->second;
    int val1, val2;
    val1 = (net->*mem)(n1);
    val2 = (net->*mem)(n2);
    if( this_edge->connects(n1, n2) ) {
      _edge_dist[ std::pair<int,int>(val1, val2) ] =
            _edge_dist[ std::pair<int,int>(val1, val2) ]  + 1;
    }
    if( this_edge->connects(n2, n1) ) {
      //If it connects in both directions, we count the edge twice:
      _edge_dist[ std::pair<int,int>(val2, val1) ] =
            _edge_dist[ std::pair<int,int>(val2, val1) ]  + 1;
    }
  }
  if( del_ei ) {
    delete ei;
  }
}

int IntStats::getCount() const { return _calls; }

const std::map<int, int>& IntStats::getDist() const { return _dist; }

double IntStats::getEdgeCorrelation() const
{
  /*
   * We need to use doubles because we can easily have sums
   * which exceed the maximum size of an integer.
   */
	
  double j,k,count,sum_j, sum_k, sum_jk, sum_j2, sum_k2;
  sum_j = 0;
  sum_k = 0;
  sum_jk = 0;
  sum_j2 = 0;
  sum_k2 = 0;
  //Add the neighbors:
  std::map< std::pair<int, int>, int>::const_iterator it;
  int total = 0;
  for( it = _edge_dist.begin(); it != _edge_dist.end(); it++) {
    j = (double)it->first.first;
    k = (double)it->first.second;
    total+= it->second;
    count = (double)it->second;
    //DEBUG:
    //DEBUG_MSG("j: " << j << " k: " << k);
    sum_j += count * j;
    sum_k += count * k;
    sum_jk += count * j * k;
    sum_j2 += count * j * j;
    sum_k2 += count * k * k;
  }
  double m_inv = 1.0 / (double)(total);
  double r, t;

  /*
   * r = frac{M^{-1}(sum_i j_i k_i) - \left(M^{-1}sum_i frac{j_i + k_i}{2}\right)^{2}}
   *          {M^{-1}sum_ifrac{j_i^2 k_i^2}{2} - \left(M^{-1}sum_i frac{j_i + k_i}{2}\right)^2}
   */
  t = m_inv * 0.5 * (double)(sum_j + sum_k);
  r = (m_inv * (double)(sum_jk) - t*t) / (m_inv * 0.5 * (double)(sum_j2 + sum_k2) - t*t);
  /*
  //DEBUG:
  cerr << "sum_j: " << sum_j << endl
       << "sum_k: " << sum_k << endl
       << "sum_jk: " << sum_jk << endl
       << "sum_j2: " << sum_j2 << endl
       << "sum_k2: " << sum_k2 << endl
       << "m_inv: " << m_inv << endl
       << "edge_set.size(): " << edge_set.size() << endl
       << "t: " << t << endl
       << "r: " << r << endl;
       */
  return r;
}

void IntStats::getEdgeEntropy(double& h_s, double& h_e, double& h_es) const
{
  //Get the e_k probability distribution:
  std::map<std::pair<int,int>, int >::const_iterator it1;
  std::map<int, int> edge_degs;
  int total = 0;
  for(it1 = _edge_dist.begin(); it1 != _edge_dist.end(); it1++) {
    edge_degs[it1->first.first] += it1->second;
    total += it1->second;
  }
  
  std::map<int, int>::const_iterator it2;
  //Here is H(start):
  h_s = 0.0;
  for( it2 = edge_degs.begin(); it2 != edge_degs.end(); it2++) {
    if( it2->second > 0) {
      h_s -= it2->second * log( (double)it2->second )/log(2.0);
    }
  }
  /*
   * p_i = x_i /t
   * H = sum_i -p_i log p_i
   *   = sum_i -p_i (log x_i - log t)
   *   = log t - (1/t) sum_i x_i log x_i
   */
  h_s /= total;
  h_s += log( (double)total )/log(2.0);
  
  edge_degs.clear();
  for(it1 = _edge_dist.begin(); it1 != _edge_dist.end(); it1++) {
    edge_degs[it1->first.second] += it1->second;
  }
  
  //Here is H(start):
  h_e = 0.0;
  for( it2 = edge_degs.begin(); it2 != edge_degs.end(); it2++) {
    if( it2->second > 0) {
      h_e -= it2->second * log( (double)it2->second )/log(2.0);
    }
  }
  /*
   * p_i = x_i /t
   * H = sum_i -p_i log p_i
   *   = sum_i -p_i (log x_i - log t)
   *   = log t - (1/t) sum_i x_i log x_i
   */
  h_e /= total;
  h_e += log( (double)total )/log(2.0);
 
  //Here is H(e_ij):
  h_es = 0.0;
  for(it1 = _edge_dist.begin(); it1 != _edge_dist.end(); it1++) {
      if( it1->second > 0 ) {
        h_es -= it1->second * log( (double) it1->second )/log(2.0);
      }
  }
  h_es /= total;
  h_es += log( total )/log(2.0);
}

int IntStats::getMax() const { return _max; }

int IntStats::getMin() const { return _min; }

int IntStats::getSum() const { return _sum; }

int IntStats::getSquaredSum() const {return _sum2; }

double IntStats::getAverage() const { return (double)_sum/(double)_calls; }

double IntStats::getEntropy() const
{
  std::map<int, int>::const_iterator deg_it;
  double entropy = 0;
  for(deg_it = _dist.begin(); deg_it != _dist.end(); deg_it++) {
    entropy -= deg_it->second * log( (double)deg_it->second )/log(2.0);
  }
  entropy /= _calls;
  entropy += log( (double)_calls )/log(2.0);
  return entropy;
}

double IntStats::getMoment2() const {return (double)_sum2/(double)_calls; }

double IntStats::getMoment(double m) const
{
  std::map<int, int>::const_iterator it;
  double tot = 0.0;
  for(it = _dist.begin(); it != _dist.end(); it++) {
    double val = (double)it->first;
    double count = (double)it->second;
    tot += pow(val, m) * count;
  }
  tot = tot/(double)_calls;
  return tot;
}

double IntStats::getVariance() const {
  double m1 = getAverage();
  double m2 = getMoment2();
  return (m2 - m1*m1);
}
