#include <netmodeler.h>
#include <fstream>
                                                                                  
using namespace std;
using namespace Starsky;

void printCommunities(string prefix, const Network& net) {
    stringstream community;
    community << prefix << ".";
    //Print out the communities:
    vector< pair<int, int> > joins;
    vector<double> q_t;
    int step = net.getCommunities(q_t, joins);
    if( q_t[step] > 0.25 ) {
      cout << "#" << prefix << "=" << q_t[step] << endl;
      set< Network > comms = net.getCommunity(step, joins);
      set< Network >::const_reverse_iterator comit;
      Network::NodePSet::const_iterator comnodeit;
      int com = 0;
      for(comit = comms.rbegin();
	  comit != comms.rend();
	  comit++) {
        stringstream this_com;
        this_com << community.str() << com++;
        cout << this_com.str();
        Network::NodePSet::const_iterator comnodeit;
        for(comnodeit = comit->getNodes().begin();
	    comnodeit != comit->getNodes().end();
	    comnodeit++) {
          cout << " " << (*comnodeit)->toString();
        }
        cout << endl;
	//Recurse:
	printCommunities(this_com.str(), *comit);
      }
    }
}

int main(int argc, char* argv) {

  Network my_net(cin);
  //WeightedNetwork my_net(cin);
  //Look on components:
  set<Network> components = my_net.getComponents();
  set<Network>::const_reverse_iterator comp_it;
  int community = 0;
  for(comp_it = components.rbegin(); comp_it != components.rend(); comp_it++) {
    stringstream com;
    com << community++;
    //printCommunities(com.str(), WeightedNetwork( *comp_it) );
    printCommunities(com.str(), *comp_it );
  }
  return 1;
}
