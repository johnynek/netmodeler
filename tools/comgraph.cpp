#include <netmodeler.h>
#include <fstream>

#include <algorithm>

#include "optionparser.h"

using namespace std;
using namespace Starsky;

#define FOREACH(it, col) for(it = col.begin(); it != col.end(); it++)

#define DEBUG 0

long expected_comnorm(set<Network*>* part)
{
  set<Network*>::iterator pit;
  long nodes = 0, norm = 0;
  int temp_node;
  FOREACH(pit, (*part)) {
    temp_node = (*pit)->getNodes().size();
    nodes += temp_node;
    norm += temp_node * temp_node;
  }
  return (norm - nodes)/2;
}

void printCommunities(AgglomPart* ap, ostream& out, string prefix, const Network& net) {
    stringstream community;
#ifdef DEBUG
    cout << prefix << endl;
#endif
    community << prefix << ".";
    //Print out the communities:
    vector< pair<int, int> > joins;
    vector<double> q_t;
    int step = ap->getCommunities(net, q_t, joins);
    /*
    for(int i = 0; i < q_t.size(); i++) {
      cout << "q_t[" << i << "] = " << q_t[i] <<
	      " join(" << joins[i].first << ", " << joins[i].second << ")" << endl;;
    }
    cout << "stepmax: " << step << endl;
   */
   //if( q_t[step] > 0.25 ) {
   if( net.getNodes().size() > 1 ) {
      out << "#" << prefix << "=" << q_t[step] << endl;
      //cout << "Getting best split"<< endl;
      set< Network* >* comms = ap->getCommunity(net, step, joins);
#if 1
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
      //Double check the modularity:
      double qmod = ap->modularityOf(comms, net);
      cout << "mod: " << qmod << endl;
      //cout << "Got best split"<< endl;
      vector<Network*> vcoms;
      vcoms.insert(vcoms.begin(), comms->begin(), comms->end());
      //Sort the vector
      //networkprt_gt is in network.h
      sort(vcoms.begin(), vcoms.end(), networkptr_gt());
      vector< Network* >::iterator comit;
      Network::NodePSet::const_iterator comnodeit;
      int com = 0;
      for(comit = vcoms.begin();
	  comit != vcoms.end();
	  comit++) {
        stringstream this_com;
        this_com << community.str() << com++;
        out << this_com.str();
        Network::NodePSet::const_iterator comnodeit;
	Network* this_comnet = *comit;
        for(comnodeit = this_comnet->getNodes().begin();
	    comnodeit != this_comnet->getNodes().end();
	    comnodeit++) {
          out << " " << (*comnodeit)->toString();
        }
        out << endl;
	//Recurse:
	//printCommunities(ap, out, this_com.str(), *this_comnet);
      }
      //Free up the memory
      ap->deletePartition(comms);
    }
}


int main(int argc, char* argv[]) {

  OptionParser op;
  
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
  
  map<string, string> popts;
  try { 
    popts = op.getOpts(argc, argv, reqs, opts);
  }
  catch (exception x) {
    cout << "usage: " << argv[0] << op.getUsageString(reqs,opts) << endl;
    return -1;
  } 
  map<string,string>::iterator mit;
  
  int iterations = 1;
  mit = popts.find("iterations");
  if( mit != popts.end() ) {
    //We have this option:
    iterations = atoi( popts["iterations"].c_str() );
  }
  int seed = -1;
  mit = popts.find("seed");
  if( mit != popts.end() ) {
    //We have this option:
    seed = atoi( popts["seed"].c_str() );
  }
  
  double prob;
  mit = popts.find("prob");
  if( mit != popts.end() ) {
    //We have this option:
    prob = atof( popts["prob"].c_str() );
  }
  Ran1Random r(seed);
    /**
     * Here is where we select which community finding algorithm to use
     */
  AgglomPart* comfinder = 0;
  if( popts["method"] == "Newman" ) {
    comfinder = new NewmanCom();
  }
  else {
    comfinder = new RandAgPart(r, popts["method"] ,prob);
  }
  
  //Here we can choose what kind of network to use:
  NetworkFactory* nf;
  mit = popts.find("weighted");
  if( mit != popts.end() ) {
    nf = new WeightedNetworkFactory();
    comfinder->useWeights(true);
  }
  else {
    //Here is unweighted:
    nf = new NetworkFactory();
  }
  Network* net;
  if( popts["input"] == "-" ) {
    net = nf->create(cin);
  }
  else {
    ifstream input( popts["input"].c_str() );
    net = nf->create(input);
  }
  Network& my_net = *net;
 
  ComponentPart cp;
  set<Network*>* comms = cp.partition(my_net);
  vector<Network*> vcoms;
  vcoms.insert(vcoms.begin(), comms->begin(), comms->end());
  //Sort the vector
  sort(vcoms.begin(), vcoms.end(), networkptr_gt());
  //We also want to compare the output to the Newman:
  NewmanCom ncom;
  while(iterations-- > 0 ) {
    stringstream name;
    name << popts["outfile"] << "_com_" << popts["method"] << "." << iterations;
    cout << name.str() << endl;
    ofstream out(name.str().c_str());
    //Look on components:
    vector< Network* >::iterator comit;
    int community = 0;
    for(comit = vcoms.begin(); comit != vcoms.end(); comit++) {
      stringstream com;
      com << community++;
      Network* this_component = *comit;
      
      
      //Recursively print the communities
      printCommunities(comfinder, out, com.str(), *this_component );
      //Print Newman:
      out << "#Newman::" << endl;
      printCommunities(&ncom, out, com.str(), *this_component );
    }
  }
  //Delete the memory we allocated:
  cp.deletePartition(comms);
  delete nf;
  delete net;
  delete comfinder;
  return 1;
}
