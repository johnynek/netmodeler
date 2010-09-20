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
  
  Ran1Random rand(seed);
  vector<int> cluster_cnts;
  cluster_cnts.resize(n_cluster, size_cluster);
  //Here is the partitioning:
  int iter = op.getIntOpt("iter",1);
  double accuracy = 0.0;
  double ave_d = 0.0;
  double information = 0.0;
  for(int i = 0; i < iter; i++) {
    INetworkPartitioner* comfinder = 0;
    if( op.getStringOpt("method", "") == "Newman" ) {
      comfinder = new NewmanCom();
    }
    else if ( op.getStringOpt("method", "") == "InfoCom" ) {
      comfinder = new InfoCom();
    }
    else if ( op.getStringOpt("method", "") == "ComponentPart" ) {
      comfinder = new ComponentPart();
    }
    else if ( op.getStringOpt("method", "") == "ClusterPart" ) {
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
    //cout << "#node count: " << rcn.getNodeSize() << endl; 
    ave_d += (2.0*rcn.getEdgeSize())/(double)rcn.getNodeSize();
    //Now run the clustering:
    vector<Network*>* part = comfinder->partition(rcn);
    double acc = rcn.accuracyOfPartition(part);
    double inf = rcn.informationOfPartition(part);
    //cout << "#acc = " << acc << endl;
    //cout << "#inf = " << inf << endl;
    accuracy += acc;
    information += inf;
    comfinder->deletePartition(part);
    delete comfinder;
  }
  cout << "accuracy = "<<(accuracy/iter) << endl; 
  cout << "information = "<<(information/iter) << endl; 
  cout << "#ave_d=" << ave_d / iter << endl;
}
