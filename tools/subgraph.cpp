#include <netmodeler.h>
#include <sstream>
#include <iostream>
#include "optionparser.h"

using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

  vector<string> reqs;
  reqs.push_back("input");
  reqs.push_back("subgraph_nodes");
  reqs.push_back("out-prefix");
 
  vector<string> opts;
  opts.push_back("weighted");
  opts.push_back("first_is_name");
  OptionParser op(reqs,opts);
  try {
    op.parse(argc, argv);
  }
  catch (exception x) {
    cerr << "usage: " << argv[0] << op.getUsageString() << endl;
    return -1;
  }
  NetworkFactory* nf;
  NodeFactory* node_f = new NamedNodeFactory();
  bool weighted = op.getBoolOpt("weighted", false);
  if( weighted ) {
    nf = new WeightedNetworkFactory(node_f,
                                    new WeightedEdgeFactory());
  }
  else {
    //Here is unweighted:
    nf = new NetworkFactory(node_f,
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
  //Subgraph
  ifstream sg( op.getStringOpt("subgraph_nodes","").c_str() );
  bool first_is_name = op.getBoolOpt("first_is_name", false);  

  SuperString line;
  int line_cnt = 0;
  while(getline(sg, line, '\n')) {
    if( line[0] == '#' ) { //This is a comment
      continue;
    }
    vector<SuperString> parts = line.split(" ");
    string name;
    int idx = 0;
    if( first_is_name ) {
      name = parts[0]; 
      idx = 1;
    }
    else {
      stringstream ss;
      ss << line_cnt;
      name = ss.str();
    }
    Network* sg_net;
    if( weighted ) { sg_net = new WeightedNetwork(); }
    else { sg_net = new Network(); }
    for(int i = idx; i < parts.size(); i++) {
      sg_net->add( node_f->create(parts[i]) );
    }
    sg_net->addJoiningEdgesFrom(net);
    ofstream out((op.getStringOpt("out-prefix","") + name).c_str());
    sg_net->printTo(out);
    delete sg_net; 
    line_cnt += 1;
  }
  
}
