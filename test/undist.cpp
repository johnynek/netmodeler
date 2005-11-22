#include <netmodeler.h>

using namespace Starsky;

int main(int argc, char* argv[]) {

  Network* net = new Network(std::cin);
  Ran1Random r;
  UniformNodeSelector uns(r);
  uns.selectFrom(net);
  for(int i = 0; i < net->getNodeSize(); i++) {
    Node* n1 = uns.select();
    Node* n2 = uns.select();
    int dist = net->getDistance(n1, n2);
    std::cout << n1->toString() << " -> " << n2->toString() 
	    << " = " << dist << std::endl;
  }
  delete net;
  return 0;
}
