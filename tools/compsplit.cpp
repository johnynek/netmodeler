#include <netmodeler.h>
#include <fstream>
#include <sstream>                                                                                
using namespace std;
using namespace Starsky;

int main(int argc, char* argv[]) {

  //Here is the input file name:
  ifstream input(argv[1]);
  Network my_net(input);
  
  set<Network> components = my_net.getComponents();
  set<Network>::const_reverse_iterator comp_it;
  int component = 0;
  for(comp_it = components.rbegin(); comp_it != components.rend(); comp_it++) {
    stringstream name;
    name << argv[1] << "." << component;
    component++;
    ofstream output(name.str().c_str());
    comp_it->printTo(output);
  }
  return 1;
}
