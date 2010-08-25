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
#include <fstream>

#include <algorithm>

#include "optionparser.h"

using namespace std;
using namespace Starsky;

#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

#define DEBUG 1

long expected_comnorm(set<Network*>* part)
{
  set<Network*>::iterator pit;
  long nodes = 0, norm = 0;
  int temp_node;
  FOREACH(pit, (*part)) {
    temp_node = (*pit)->getNodeSize();
    nodes += temp_node;
    norm += temp_node * temp_node;
  }
  return (norm - nodes)/2;
}

void printCommunities(INetworkPartitioner* ap,
		      ostream& out, string prefix, const Network& net) {
    stringstream community;
#ifdef DEBUG
    cout << prefix << endl;
#endif
    community << prefix << ".";
    //Print out the communities:
   vector<Network*>* vcoms = ap->partition(net);
   double mod = ap->modularityOf(vcoms, net);
   //if( true ) 
   if( mod > 0.2 ) 
   //if( net.getNodeSize() > 1 ) 
   {
      out << "#" << prefix << "=" << mod << endl;
      //cout << "Getting best split"<< endl;
#if 0
      //Compute the distance to the Newman partition:
      NewmanCom ncom;
      set<Network*>* newman_c = ncom.partition( net );
      long n_m, c_m;
      long dist = ncom.distance(newman_c, comms, n_m, c_m);
      out << "#|C| = " << c_m << endl << "#|N| = " << n_m << endl << "#|N-C| = " << dist << endl;
      out << "#Exp(|C|) = " << expected_comnorm(comms) << endl << "#Exp(|N|) = "
              << expected_comnorm(newman_c) << endl;
      ncom.deletePartition(newman_c);
#endif
      cout << "mod: " << mod << endl;
      //cout << "Got best split"<< endl;
      vector< Network* >::iterator comit;
      Network::NodePSet::const_iterator comnodeit;
      int com = 0;
      for(comit = vcoms->begin();
	  comit != vcoms->end();
	  comit++) {
        stringstream this_com;
        this_com << community.str() << com++;
        out << this_com.str();
        Network::NodePSet::const_iterator comnodeit;
	Network* this_comnet = *comit;
	auto_ptr<NodeIterator> ni( this_comnet->getNodeIterator() );
	while( ni->moveNext() ) {
          out << " " << ni->current()->toString();
        }
        out << endl;
	//Recurse:
	//printCommunities(ap, out, this_com.str(), *this_comnet);
      }
    }
    //Free up the memory
    ap->deletePartition(vcoms);
}


int main(int argc, char* argv[]) {

  //Here are the default ordering of options:
  vector<string> reqs;
  //This are required:
  reqs.push_back("input");
  reqs.push_back("outfile");
  reqs.push_back("method");
  vector<string> opts;
  //These are optional:
  opts.push_back("iterations");
  opts.push_back("seed");
  opts.push_back("prob");
  opts.push_back("weighted");
  
  OptionParser op(reqs, opts);
  try { 
    op.parse(argc, argv);
  }
  catch (exception x) {
    cout << "usage: " << argv[0] << op.getUsageString() << endl;
    return -1;
  } 
  int iterations = op.getIntOpt("iterations", 1);
  int seed = op.getIntOpt("seed", -1);
  double prob = op.getDoubleOpt("prob", 0.0);
  Ran1Random r(seed);
    /**
     * Here is where we select which community finding algorithm to use
     */
  INetworkPartitioner* comfinder = 0;
  if( op.getStringOpt("method", "") == "Newman" ) {
    comfinder = new NewmanCom();
  }
  else if ( op.getStringOpt("method", "") == "ClusterPart" ) {
    comfinder = new ClusterPart(prob);
  }
  else {
    comfinder = new RandAgPart(r, op.getStringOpt("method","") ,prob);
  }
  
  //Here we can choose what kind of network to use:
  NetworkFactory* nf;
  if( op.getBoolOpt("weighted", false) ) {
    nf = new WeightedNetworkFactory(new NamedNodeFactory(),
                                    new WeightedEdgeFactory());
    comfinder->useWeights(true);
  }
  else {
    //Here is unweighted:
    nf = new NetworkFactory(new NamedNodeFactory(),
                            new EdgeFactory());
  }
  Network* net;
  if( op.getStringOpt("input", "-") == "-" ) {
    net = nf->create(cin);
  }
  else {
    ifstream input( op.getStringOpt("input","").c_str() );
    net = nf->create(input);
  }
  delete nf;
  Network& my_net = *net;
  cout << "#loaded net" << endl; 
  ComponentPart cp;
  vector<Network*>* comms = cp.partition(my_net);
  //We also want to compare the output to the Newman:
  NewmanCom ncom;
  while(iterations-- > 0 ) {
    stringstream name;
    name << op.getStringOpt("outfile","") << "_com_" << op.getStringOpt("method","")
	    << "." << iterations;
    cout << name.str() << endl;
    ofstream out(name.str().c_str());
    //Output the command we used to produce the data.
    out << "#usage:\t" <<  op.getUsageString() << endl;
    out << "#command:\t";
    for(int i = 0; i < argc; i++) {
      out << argv[i] << " ";
    }
    out << std::endl;

    //Look on components:
    vector< Network* >::iterator comit;
    int community = 0;
    for(comit = comms->begin(); comit != comms->end(); comit++) {
      stringstream com;
      com << community++;
      Network* this_component = *comit;
      
      
      //Recursively print the communities
      printCommunities(comfinder, out, com.str(), *this_component );
      //Print Newman:
      //out << "#Newman::" << endl;
      //printCommunities(&ncom, out, com.str(), *this_component );
    }
  }
  //Delete the memory we allocated:
  cp.deletePartition(comms);
  delete net;
  delete comfinder;
  return 1;
}
