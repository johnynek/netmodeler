#include <netmodeler.h>
#include <fstream>
                                                                                  
using namespace std;
using namespace Starsky;

int main(int argc, char* argv) {

  Network my_net(cin);
  vector< pair<int, int> > joins;
  vector<double> q_t;
  NewmanCom comfinder;
  comfinder.getCommunities(my_net,q_t, joins);
  //In order to know which community refers to what, we must build a table:
  vector<Node*> all_nodes;
  all_nodes.insert( all_nodes.begin(),
		    my_net.getNodes().begin(),
		    my_net.getNodes().end() );
  
  for(int i = 0; i < joins.size(); i++) {
      int join1, join2;
      join1 = joins[i].first;
      join2 = joins[i].second;
      cout << join1 << "\t" << join2 << "\t" << q_t[i] << endl;
  }
  cout << "#<tab>id<tab>node:" << endl;
  for(int i = 0; i < all_nodes.size(); i++) {
    cout << "#\t" << i << "\t" << all_nodes[i]->toString() << endl;
  }
#if 0
  //Look on components:
  set<Network> components = my_net.getComponents();
  set<Network>::const_reverse_iterator comp_it;
  int community = 0;
  for(comp_it = components.rbegin(); comp_it != components.rend(); comp_it++) {
    comp_it->getCommunities(q_t, joins);
    for(int i = 0; i < joins.size(); i++) {
      cout << joins[i].first << "\t" << joins[i].second << "\t"
	   << q_t[i] << endl;
    }
  }
#endif
  return 1;
}
