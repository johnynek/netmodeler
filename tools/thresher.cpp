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

int main(int argc, char* argv[])
{

  vector<string> reqs;
  vector<string> opts;

  reqs.push_back("nodes");
  opts.push_back("points");
  opts.push_back("samples");
  opts.push_back("exp");
  opts.push_back("bigkmax");
  opts.push_back("indep");
  OptionParser op(reqs, opts); 
  try {
    op.parse(argc, argv);
  }
  catch (exception x) {
    cout << "usage: " << argv[0] << op.getUsageString() << endl;
    return -1;
  }
  //Output the command we used to produce the data.
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
  int k_max = nodes;
  if( ! bigkmax ) {
    k_max = (int)pow((double)nodes,1.0/exp);
  }
  PowerLawProbabilityFunction pl(-1.0*exp, 2, k_max);
  NetworkFactory* nf = new DegreeLawNetFac(nodes, pl, ran, indep);
  Network* net = nf->create();
  DEBUG("Made net");  
  map<double, double> prob_to_size;
  map<double, string> prob_to_out;
  
  map<double, double>::iterator pit, pit2;
  
  prob_to_size[0.0] = 0.0;
  prob_to_size[1.0] = (double)net->getNodes().size();
  stringstream outstr0;
  prob_to_out[0.0] = "0.0\t0\t0";
  outstr0 << 1.0 << "\t" << net->getNodes().size()
          << "\t" << net->getEdges().size();
  prob_to_out[1.0] = outstr0.str();
  ComponentPart comp_part;
  while( points-- > 0 ) {
    DEBUG( "Points: " << points );  
    //Find the biggest "area"
    double max_area, area;
    double last_p;
    double last_size;
    FOREACH(pit, prob_to_size)
    {
      if( pit == prob_to_size.begin() ) {
        max_area = 0.0;
      }
      else {
        area = abs( (pit->first - last_p)*(pit->second - last_size) ); 
        if( area > max_area ) {
          max_area = area;
          pit2 = pit;
        }
      }
      last_p = pit->first;
      last_size = pit->second;
    }
    //Now we have the biggest area.  Lets, split it:
    double this_p = pit2->first;
    pit2--;
    last_p = pit2->first;
    double new_p = (last_p + this_p)/2.0;
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
      set<Network*>* comps = comp_part.partition(tmp_net);
      Network* largest = comp_part.getLargest( comps );
      nodes_sum += largest->getNodes().size();
      edges_sum += largest->getEdges().size();
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
  }
  //We have all the points now, output the data:
  map<double, string>::iterator oit;
  FOREACH(oit, prob_to_out) {
    cout << oit->second << endl;
  }
  //Here we look for the percolation threshold:
  double last_q = 0.0;
  double max_slope = 0.0;
  double q_thresh = 0.0;
  double last_s = 0.0;
  FOREACH(pit, prob_to_size) {
    if( pit->second > last_s ) {
      double slope = (pit->second - last_s)/(pit->first - last_q);
      if( slope > max_slope ) {
        q_thresh = last_q;
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
  last_q = 1.0;
  last_s = (double)net->getNodes().size();
  max_slope = 0.0;
  map<double, double>::reverse_iterator rit;
  double q_thresh2;
  for(rit = prob_to_size.rbegin();
      rit != prob_to_size.rend();
      rit++)
  {
    if( rit->second < last_s ) {
      double slope = (rit->second - last_s)/(rit->first - last_q);
      if( slope > max_slope ) {
        q_thresh2 = last_q;
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
  cout << "#q_th: " << q_thresh << ", " << q_thresh2 << ", "
        << (q_thresh+q_thresh2)/2.0 << endl;
  nf->deleteNet(net);
  delete nf;
}
