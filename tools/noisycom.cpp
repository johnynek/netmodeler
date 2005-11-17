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

#include <netmodeler.h>
#include "optionparser.h"

/**
 * this is a simple tool that takes a graph, rewires edges,
 * and then plots how strong the communities are, as a function
 * of how strong the rewiring is
 */

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

  vector<string> reqs;
  //Here are the default ordering of options:
  reqs.push_back("input");
  reqs.push_back("mapper");
  reqs.push_back("comfinder");
  vector<string> opts;
  map<string, string> popts;
  try { 
    popts = OptionParser::getOpts(argc, argv, reqs, opts);
  }
  catch(exception x) {
    cerr << x.what() << endl;
    cout << "usage: " << argv[0] << OptionParser::getUsageString(reqs,opts) << endl;
    return 1;
  }
  Ran1Random ran;
  
  INetworkPartitioner* comfinder = 0;
  if( popts["comfinder"] == "Newman" ) {
    comfinder = new NewmanCom();
  }
  else {
    comfinder = new RandAgPart(ran, popts["comfinder"]);
  }
  NetworkFactory* nf = new NetworkFactory();
  Network* net;
  if( popts["input"] == "-") {
    net = nf->create(cin);
  }
  else {
    ifstream input( popts["input"].c_str() );
    net = nf->create(input);
  }

  double p = 0;
  EdgeFactory ef;
  for(p = 0.0; p < 1.0; p += 0.01) {
    Network net_copy = *net;
    INetworkMapper* mapper = 0;
    if( popts["mapper"] == "EdgeRewirer" ) {
      mapper = new EdgeRewirer(ef, p, ran);
    }
    else if ( popts["mapper"] == "EdgeSwapper" ) {
      mapper = new EdgeSwapper(ef, ran, p);
    }
    mapper->map(&net_copy);
    vector<Network*>* part = comfinder->partition(net_copy);
    double q = comfinder->modularityOf(part, *net);
    cout << p << "\t" << q << endl;
    comfinder->deletePartition(part);
    delete mapper;
  }
  return 0;
}
