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

/**
 * This code tests bond percolation on networks at various sizes
 * near the threshold
 */

#include <netmodeler.h>
#include <algorithm>

#include <cstdlib>

#include "optionparser.h"

using namespace std;
using namespace Starsky;


int main(int argc, char* argv[])
{
  OptionParser op;
  vector<string> reqs;
  vector<string> opts;

  reqs.push_back("maxnodes");
  reqs.push_back("iterations");
  opts.push_back("exp");
  opts.push_back("perceps");
  opts.push_back("bigkmax");
  opts.push_back("indep");
  map<string, string> popts;
  map<string, string>::iterator mit;
  try {
    popts = op.getOpts(argc, argv, reqs, opts);
  }
  catch (exception x) {
    cout << "usage: " << argv[0] << op.getUsageString(reqs,opts) << endl;
    return -1;
  }
  int max_nodes = atoi( popts["maxnodes"].c_str() );
  int count = atoi( popts["iterations"].c_str() );
  //Here are the optional ones:
  mit = popts.find("exp");
  double exp = 2.0;
  if( mit != popts.end() ) {
    exp = atof( mit->second.c_str() ); 
  }
  double eps = 1.0; //q = 2q_c 
  mit = popts.find("perceps");
  if( mit != popts.end() ) {
    eps = atof( mit->second.c_str() ); 
  }
  bool bigkmax = false;
  mit = popts.find("bigkmax");
  if( mit != popts.end() ) {
    bigkmax = atoi( mit->second.c_str() ) > 0;
  }
  bool indep = false;
  mit = popts.find("indep");
  if( mit != popts.end() ) {
    indep = atoi( mit->second.c_str() ) > 0;
  }
  cout << "#command:\t";
  for(int i = 0; i < argc; i++) {
    cout << argv[i] << " ";
  }
  cout << endl;
  cout << "#maxnodes: " << max_nodes << endl
       << "#exp: " << exp << endl
       << "#perceps: " << eps << endl
       << "#interations: " << count << endl
       << "#indep: " << indep << endl
       << "#bigkmax: " << bigkmax << endl;
  //////////////////////////////////////////////////
  cout << "#nodes\tn_largest\tperc_edges\te_largest\tpre_perc_edges\tk_max\tmax_deg" << endl;
  Ran1Random ran;
  int nodes = 1000;
  //This is an INetworkPartioner that just
  //looks for all the components
  ComponentPart cp;
  //Here is a bond percolator:
    
  for(int steps = 0; nodes < max_nodes; steps++) {
    nodes = (int)(nodes * 1.05);
    //Build the network
    int size = 0;
    int edges = 0;
    int edges_comp = 0;
    int pre_perc_edges = 0;
    int k_max = nodes;
    if( ! bigkmax ) {
      k_max = (int)pow((double)nodes,1.0/exp);
    }
    PowerLawProbabilityFunction pl(-1.0*exp,2, k_max);
    NetworkFactory* nf = new DegreeLawNetFac(nodes, pl, ran, indep);
    int max_deg = -1;
    for(int k = 0; k < count; k++) {
      Network* net = nf->create();
      //Get the percolation threshold:
      double k1 = net->getDegreeMoment(1);
      double k2 = net->getDegreeMoment(2);
      double q_c = k1/(k2 - k1);
      double q = q_c * (1.0 + eps);
      INetworkMapper* bp = new PercolationMapper(ran, q);
   
      pre_perc_edges += net->getEdges().size();
      node_i_func out = net->forEachNode(node_i_func(net, &Network::getDegree) );
      max_deg = out.maxVal; 
      bp->map(net);
      //Here are the number of edges that remain after percolation
      edges += net->getEdges().size();
      //Get the components:
      set<Network*>* comps = cp.partition(*net);
      //Here are the number of nodes in the largest component
      Network* largest = cp.getLargest( comps );
      size += largest->getNodes().size();
      //Here are the number of edges in the largest component.
      edges_comp += largest->getEdges().size();
      cp.deletePartition(comps);
      nf->deleteNet(net);
      delete bp;
    }
    
    double ave_size = (double)size/(double)count;
    double ave_edge = (double)edges/(double)count;
    double ave_edge_comp = (double)edges_comp/(double)count;
    double ave_pre_perc_edges = (double)pre_perc_edges/(double)count;
    cout << nodes << "\t" << ave_size << "\t"
	 << ave_edge << "\t" << ave_edge_comp
	 << "\t" << ave_pre_perc_edges << "\t" << k_max << "\t" << max_deg << endl;
    delete nf;
  }
  return 0;
}
