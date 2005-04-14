#include <netmodeler.h>
#include <fstream>

#include <algorithm>

using namespace std;
using namespace Starsky;

void printCommunities(AgglomPart* ap, ostream& out, string prefix, const Network& net) {
    stringstream community;
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
    if( q_t[step] > 0.25 ) {
      out << "#" << prefix << "=" << q_t[step] << endl;
      //cout << "Getting best split"<< endl;
      set< Network* >* comms = ap->getCommunity(net, step, joins);
      //Double check the modularity:
      double qmod = ap->modularityOf(*comms, net);
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
	printCommunities(ap, out, this_com.str(), *this_comnet);
      }
      //Free up the memory
      ap->deletePartition(comms);
    }
}

int main(int argc, char* argv[]) {

  if( argc < 4 ) {
    cout << "usage: " << argv[0]
	 << " input_file.nm outfile method [iterations] [seed] [prob]" << endl;
    return 0;
  }
  
  Network* net;
  if( string(argv[1]) == string("-") ) {
    net = new Network(cin);
  }
  else {
    ifstream input(argv[1]);
    net = new Network(input);
  }

  int iterations = 1;
  if( argc > 4 ) {
    iterations = atoi(argv[4]);
  }
  int seed = -1;
  if( argc > 5 ) {
    seed = atoi(argv[5]);
  }
  double prob;
  if( argc > 6 ) {
    prob = atof(argv[6]);
  }
  else {
    prob = 0.0;
  }
  Ran1Random r(seed);
    /**
     * Here is where we select which community finding algorithm to use
     */
  AgglomPart* comfinder = 0;
  if( string(argv[3]) == "Newman" ) {
    NewmanCom nm;
    comfinder = &nm;
  }
  else {
    RandAgPart rap(r,argv[3],prob);
    comfinder = &rap;
  }
 
  Network& my_net = *net;
 
  ComponentPart cp;
  set<Network*>* comms = cp.partition(my_net);
  vector<Network*> vcoms;
  vcoms.insert(vcoms.begin(), comms->begin(), comms->end());
  //Sort the vector
  sort(vcoms.begin(), vcoms.end(), networkptr_gt());
  while(iterations-- > 0 ) {
    stringstream name;
    name << argv[2] << "_com_" << argv[3] << "." << iterations;
    cout << name.str() << endl;
    ofstream out(name.str().c_str());
    //Look on components:
    vector< Network* >::iterator comit;
    int community = 0;
    for(comit = vcoms.begin(); comit != vcoms.end(); comit++) {
      stringstream com;
      com << community++;
      Network* this_component = *comit;
      printCommunities(comfinder, out, com.str(), *this_component );
    }
  }
  cp.deletePartition(comms);
  return 1;
}
