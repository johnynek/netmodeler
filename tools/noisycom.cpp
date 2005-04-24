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

  OptionParser op;

  vector<string> reqs;
  //Here are the default ordering of options:
  reqs.push_back("input");
  reqs.push_back("mapper");
  reqs.push_back("comfinder");
  vector<string> opts;
  map<string, string> popts;
  try { 
    popts = op.getOpts(argc, argv, reqs, opts);
  }
  catch(exception x) {
    cerr << x.what() << endl;
    cout << "usage: " << argv[0] << op.getUsageString(reqs,opts) << endl;
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
    set<Network*>* part = comfinder->partition(net_copy);
    double q = comfinder->modularityOf(*part, *net);
    cout << p << "\t" << q << endl;
    comfinder->deletePartition(part);
    delete mapper;
  }
  return 0;
}
