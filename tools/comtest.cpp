/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2010  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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

#include <netmodeler.h>
#include <fstream>

#include "optionparser.h"

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

  vector<string> reqs;
  reqs.push_back("n_clust");
  reqs.push_back("size_clust");
  reqs.push_back("exp_d");
  reqs.push_back("out-in-ratio");
  reqs.push_back("method");
  vector<string> optionals;
  optionals.push_back("seed");
  optionals.push_back("iter");
  optionals.push_back("min_cc");
  optionals.push_back("sasteps");
  OptionParser op(reqs, optionals);
  try {
    op.parse(argc, argv);
  }
  catch (exception x) {
    cout << "usage: " << argv[0] << op.getUsageString() << endl;
    return -1;
  }
  int n_cluster = op.getIntOpt("n_clust", 0);
  int size_cluster = op.getIntOpt("size_clust", 0);
  double exp_d = op.getDoubleOpt("exp_d", 0.0);
  double r = op.getDoubleOpt("out-in-ratio", 0.0);
  int seed = op.getIntOpt("seed", -1);
  string method = op.getStringOpt("method",""); 
  Ran1Random rand(seed);
  vector<int> cluster_cnts;
  cluster_cnts.resize(n_cluster, size_cluster);
  //Here is the partitioning:
  int iter = op.getIntOpt("iter",1);
  double accuracy = 0.0;
  double ave_d = 0.0;
  double information = 0.0;
  double cominf = 0.0;
  double c_inf = 0.0;
  double part_inf = 0.0;
  double c_cominf = 0.0;
  double mod = 0.0;
  double c_mod = 0.0;
  for(int i = 0; i < iter; i++) {
    cnt_ptr<INetworkPartitioner> comfinder;
    if( method == "Newman" ) {
      comfinder = new NewmanCom();
    }
    else if (method == "InfoCom" ) {
      comfinder = new InfoCom();
    }
    else if ( method == "SAPartitioner" ) {
      //ModularityEnergy energy;
      //ComInfoEnergy energy;
      InformationEnergy energy;
      int sasteps = op.getIntOpt("sasteps", -1);
      comfinder = new SAPartitioner(rand, energy, sasteps);
    }
    else if ( method == "ComponentPart" ) {
      comfinder = new ComponentPart();
    }
    else if ( method == "ClusterPart" ) {
      double min_cc = op.getDoubleOpt("min_cc", -1.0);
      if( min_cc < 0.0 ) {
        cerr << "option min_cc required >= 0.0" << endl;
        return -1;    
      }
      comfinder = new ClusterPart(min_cc);
    }
    else {
      cerr << "Unrecognized method: " << op.getStringOpt("method","unknown");
      return -1;
    }
    RandCompNetwork rcn(cluster_cnts, exp_d, r, rand);
    const cnt_ptr<NetworkPartition>& correct = rcn.correctPartition();
    //cout << "#node count: " << rcn.getNodeSize() << endl; 
    ave_d += (2.0*rcn.getEdgeSize())/(double)rcn.getNodeSize();
    //Now run the clustering:
    auto_ptr<NetworkPartition> part(comfinder->partition(rcn));
    double acc = rcn.accuracyOfPartition(part.get());
    double inf = rcn.informationOfPartition(part.get());
    //cout << "#acc = " << acc << endl;
    //cout << "#inf = " << inf << endl;
    accuracy += acc;
    information += inf;
    mod += part->getModularity();
    c_mod += correct->getModularity();
    c_inf += InfoCom::informationOf(correct.get());
    part_inf += InfoCom::informationOf(part.get());
    cominf += part->getComInformation();
    c_cominf += correct->getComInformation();
  }
  vector< pair<string, double> > results;
  results.push_back( pair<string, double>("accuracy", (accuracy/iter)));
  results.push_back( pair<string, double>("information", (information/iter)));
  results.push_back( pair<string, double>("modularity", (mod/iter)));
  results.push_back( pair<string, double>("correct modularity", (c_mod/iter)));
  results.push_back( pair<string, double>("cominfo", (cominf/iter)));
  results.push_back( pair<string, double>("correct cominfo", (c_cominf/iter)));
  results.push_back( pair<string, double>("I(C;E)", (part_inf/iter)));
  results.push_back( pair<string, double>("correct I(C;E)", (c_inf/iter)));
  results.push_back( pair<string, double>("<d>", (ave_d/iter)));
  cout << "#";
  for(vector< pair<string, double> >::iterator rit = results.begin();
      rit != results.end(); rit++) {
    cout << rit->first << ", ";
  }
  cout << endl;
  for(vector< pair<string, double> >::iterator rit = results.begin();
      rit != results.end(); rit++) {
    cout << rit->second << ", ";
  }
  cout << endl;
}
