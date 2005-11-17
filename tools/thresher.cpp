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

/**
 * This is a program to find percolation thresholds 
 * in graphs.  It adaptively sets percolation probabilities
 * to space points more closely in regions of large change.
 */

#include <netmodeler.h>
#include "optionparser.h"

#include <map>
#include <fstream>
#include <cmath>

#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

//#define DEBUG(msg) std::cout << msg << std::endl;
#define DEBUG(msg) 

using namespace Starsky;
using namespace std;


/**
 * This function looks through the prob_to_size map to find the 
 * threshold.  We define the threshold as the point of maximum dN/dq.
 *
 * @param qhigh the estimate of the threshold using the increasing filter.
 * @param qlow the estimate of the threshold using the decreasing filter
 * @return the average of the above.
 */
double getThreshold(map<double, double>& prob_to_size, double& qhigh, double& qlow)
{
  //Here we look for the percolation threshold:
  map<double, double>::iterator pit;
  pit = prob_to_size.begin();
  double last_q = pit->first;
  double last_s = pit->second;
  double max_slope = 0.0;
  qhigh = 0.0;
  FOREACH(pit, prob_to_size) {
    if( pit->second > last_s ) {
      double slope = (pit->second - last_s)/(pit->first - last_q);
      if( slope > max_slope ) {
        qhigh = last_q;
        max_slope = slope;
      }
    }
    else {
      //Due to noise, the function may not be
      //monotonic.  Ignore any such non-increasing points:
    }
    last_q = pit->first;
    last_s = pit->second;
  }
  //Look for it in the opposite direction:
  map<double, double>::reverse_iterator rit = prob_to_size.rbegin();
  last_q = rit->first;
  last_s = rit->second;
  max_slope = 0.0;
  qlow = 0.0;
  for(rit = prob_to_size.rbegin();
      rit != prob_to_size.rend();
      rit++)
  {
    if( rit->second < last_s ) {
      double slope = (rit->second - last_s)/(rit->first - last_q);
      if( slope > max_slope ) {
        qlow = last_q;
        max_slope = slope;
      }
    }
    else {
      //Due to noise, the function may not be
      //monotonic.  Ignore any such non-decreasing points:
    }
    last_q = rit->first;
    last_s = rit->second;
  }
  return (qhigh + qlow)/2.0;
}

/**
 * Do an adaptive percolation simulation
 */
void getThresholdPoints(Network* net, int points,
                        int samples, Random& ran,
                        double start, double end,
                        map<double, double>& prob_to_size,
		        map<double, string>& prob_to_out)
{
  map<double, double>::iterator pit, pit2;
 /* 
  prob_to_size[0.0] = 0.0;
  prob_to_size[1.0] = (double)net->getNodes().size();
  prob_to_out[0.0] = "0.0\t0\t0";
  //Here is the first output:
  stringstream outstr0;
  outstr0 << 1.0 << "\t" << net->getNodes().size()
          << "\t" << net->getEdges().size();
  prob_to_out[1.0] = outstr0.str();
  */
  ComponentPart comp_part;
  //This makes finding the maximum area faster:
  map<double, pair<double, double> > area_map;
  int runs = 0;
  while( points-- > 0 ) {
    DEBUG( "Points: " << points );  
    double new_p, low_p, high_p;
    if( runs == 0 ) {
      //The first time, we use 
      new_p = start;
    }
    else if ( runs == 1 ) {
      new_p = end;
    }
    else if ( runs == 2 ) {
      //Initialize the area_map:
      double a0 = abs( (prob_to_size.rbegin()->first - prob_to_size.begin()->first)
	       * (prob_to_size.rbegin()->second - prob_to_size.begin()->second));
      area_map[a0] = pair<double,double>( prob_to_size.begin()->first,
		                   prob_to_size.rbegin()->first );

    }
    if ( runs >= 2 ) {
      //Find the biggest "area"
      map<double, pair<double, double> >::reverse_iterator ait;
      ait = area_map.rbegin();
      low_p = ait->second.first;
      high_p = ait->second.second;
      new_p = (low_p + high_p)/2.0;
      //Remove this area:
      area_map.erase(ait->first);
    }
    DEBUG( "New_p: " << new_p );  
    //Percolate with this probability:
    PercolationMapper pm(ran, new_p);
    //Do this several times to get the data:
    int nodes_sum=0, edges_sum=0;
    for(int i = 0; i < samples; i++) { 
      //Make a copy of the network:
      Network tmp_net = *net;
      DEBUG("About to map");
      pm.map( &tmp_net );
      DEBUG("About to partition");
      vector<Network*>* comps = comp_part.partition(tmp_net);
      Network* largest = (*comps)[0];
      nodes_sum += largest->getNodeSize();
      edges_sum += largest->getEdgeSize();
      //Free up the partition memory:
      comp_part.deletePartition( comps );
    }
    double node_ave = (double)nodes_sum/(double)samples;
    double edge_ave = (double)edges_sum/(double)samples;
    stringstream outstr;
    outstr << new_p << "\t" << node_ave << "\t" << edge_ave;
    DEBUG( outstr.str() );
    prob_to_out[new_p] = outstr.str();
    prob_to_size[new_p] = node_ave;
    //Add the new areas:
    if( runs >= 2 ) {
      double n_tmp = prob_to_size[low_p];
      double a_tmp = abs( (n_tmp - node_ave)*(low_p - new_p) );
      area_map[a_tmp] = pair<double,double>(low_p, new_p);
      n_tmp = prob_to_size[high_p];
      a_tmp = abs( (n_tmp - node_ave)*(high_p - new_p) );
      area_map[a_tmp] = pair<double,double>(new_p, high_p);
    }
    runs++;
  }
}

int main(int argc, char* argv[])
{
#if 1
  vector<string> reqs;
  vector<string> opts;

  reqs.push_back("nodes");
  opts.push_back("points");
  opts.push_back("samples");
  opts.push_back("exp");
  opts.push_back("bigkmax");
  opts.push_back("indep");
  opts.push_back("qmax");
  opts.push_back("qmin");
  OptionParser op(reqs, opts); 
  try {
    op.parse(argc, argv);
  }
  catch (exception x) {
    cout << "usage: " << argv[0] << op.getUsageString() << endl;
    return -1;
  }
  //Output the command we used to produce the data.
  cout << "#usage:\t" <<  op.getUsageString() << endl;
  cout << "#command:\t";
  for(int i = 0; i < argc; i++) {
    cout << argv[i] << " ";
  }
  cout << endl; 
  Ran1Random ran;
  
  int points = op.getIntOpt("points", 100); //The number of points to plot
  int samples = op.getIntOpt("samples", 3);
  double exp = op.getDoubleOpt("exp", 2.0); 
  int nodes = op.getIntOpt("nodes", 2000); 
  bool bigkmax = op.getBoolOpt("bigkmax", false);
  bool indep = op.getBoolOpt("indep", false);
  double qmin = op.getDoubleOpt("qmin", 0.0);
  double qmax = op.getDoubleOpt("qmax", 1.0);
  int k_max = nodes;
  if( ! bigkmax ) {
    k_max = (int)pow((double)nodes,1.0/exp);
  }
  PowerLawDRV pl(ran, -1.0*exp, 2, k_max);
  NetworkFactory* nf = new DegreeLawNetFac(nodes, pl, ran, indep);
  Network* net = nf->create();
  DEBUG("Made net");  
    cout << "#loaded network" << endl;	
    DEBUG("Made net");  
    IntStats ns;
    auto_ptr<NodeIterator> ni(net->getNodeIterator());
    ns.collect(net, &Network::getDegree, ni.get());
    ns.collectByEdge(net, &Network::getDegree);
    cout << "#assortativity: " << ns.getEdgeCorrelation() << endl;
    cout << "#cluster coeff: " << net->getClusterCoefficient() << endl;
    cout << "#transitivity: " << net->getTransitivity() << endl;
    cout << "#nodes: " << net->getNodeSize() << endl;
    cout << "#edges: " << net->getEdgeSize() << endl;
    cout     << "#<k>: " << ns.getAverage() << endl;
    cout     << "#<k^2>: " << ns.getMoment2() << endl;
    cout     << "#H(degs): " << ns.getEntropy() << endl;
    cout     << "#EdgeMI: " << ns.getEdgeMutualInfo() << endl;
    //Here is the expected threshold:
    double exp_th = PercolationMapper::getExpectedThreshold(net);
    cout << "#q_c: " << exp_th << endl;
  map<double, double> prob_to_size;
  map<double, string> prob_to_out;
  //Actually do the simulation:
  getThresholdPoints(net, points, samples, ran, qmin, qmax, prob_to_size, prob_to_out); 
  //We have all the points now, output the data:
  map<double, string>::iterator oit;
  FOREACH(oit, prob_to_out) {
    cout << oit->second << endl;
  }
  //Get the threshold:
  double qhigh, qlow, qthresh;
  qthresh = getThreshold(prob_to_size, qhigh, qlow);
  cout << "#q_th: " << qthresh << ", low: " << qlow << ", high: " << qhigh << endl;
  //Here is the expected threshold:
  cout << "#q_c: " << exp_th << endl;
  nf->deleteNet(net);
  delete nf;
#else
  /*
   * This code sweeps through networks of different sizes,
   * Outputting the measured and expected percolation threshold
   * at each size.
   */
  vector<string> reqs;
  vector<string> opts;

  reqs.push_back("nodes");
  opts.push_back("points");
  opts.push_back("samples");
  opts.push_back("exp");
  opts.push_back("bigkmax");
  opts.push_back("indep");
  opts.push_back("eps");
  OptionParser op(reqs, opts); 
  try {
    op.parse(argc, argv);
  }
  catch (exception x) {
    cout << "usage: " << argv[0] << op.getUsageString() << endl;
    return -1;
  }
  //Output the command we used to produce the data.
  cout << "#usage:\t" <<  op.getUsageString() << endl;
  cout << "#command:\t";
  for(int i = 0; i < argc; i++) {
    cout << argv[i] << " ";
  }
  cout << endl; 
  
  int points = op.getIntOpt("points", 100); //The number of points to plot
  int samples = op.getIntOpt("samples", 3);
  double exp = op.getDoubleOpt("exp", 2.0); 
  int nodes = op.getIntOpt("nodes", 20000); 
  bool bigkmax = op.getBoolOpt("bigkmax", false);
  bool indep = op.getBoolOpt("indep", false);
  double eps = op.getDoubleOpt("eps", 1.05);
  cout << "#points: " << points << endl;
  cout << "#samples: " << samples << endl;
  cout << "#exp: " << exp << endl;
  cout << "#bigkmax: " << bigkmax << endl;
  cout << "#indep: " << indep << endl;
  cout << "#eps: " << eps << endl;
  Ran1Random ran;
  int this_node_cnt = 100;
  double last_threshold = 1.0;
  cout << "#size\tm_thresh\tsize(m_thresh)\texp_thresh\tsize(exp_thresh)" << endl;
  while( this_node_cnt < nodes ) {
    int k_max;
    if( bigkmax ) {
      k_max = (int)pow((double)this_node_cnt,1.0/(exp-1.0));
    }
    else {
      k_max = (int)pow((double)this_node_cnt,1.0/exp);
    }
    PowerLawProbabilityFunction pl(-1.0*exp, 2, k_max);
    NetworkFactory* nf = new DegreeLawNetFac(this_node_cnt, pl, ran, indep);
    Network* net = nf->create();
    DEBUG("Made net");  
    map<double, double> prob_to_size;
    map<double, string> prob_to_out;
    //Actually do the simulation:
    double this_upper_bound = 2.0 * last_threshold;
    if( this_upper_bound > 1.0 ) {
      this_upper_bound = 1.0;
    }
    getThresholdPoints(net, points, samples, ran, 0.0, this_upper_bound,
                       prob_to_size, prob_to_out); 
    DEBUG("Got threshold points");
    //Get the threshold:
    double qhigh, qlow, qthresh;
    qthresh = getThreshold(prob_to_size, qhigh, qlow);
    DEBUG("Got threshold: " << qthresh);
    last_threshold = qthresh;
    //Here is the expected threshold:
    double exp_th = PercolationMapper::getExpectedThreshold(net);
    DEBUG("Got expected th: " << exp_th);
    //Write the output:
    double size_exp = prob_to_size.lower_bound(exp_th)->second;
    double size_th = prob_to_size.lower_bound(qthresh)->second;

    cout << this_node_cnt << "\t" << qthresh << "\t" << size_th
                          << "\t" << exp_th << "\t" << size_exp << endl;
    nf->deleteNet(net);
    delete nf;
    this_node_cnt = (int)(eps * this_node_cnt);
  }
  
#endif
}
