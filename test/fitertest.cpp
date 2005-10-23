#include <netmodeler.h>
#include <iostream>

using namespace Starsky;

class Fittest {

  public:
    Network* _net;
    Fittest(Network* n) {
      _net = n;
    }
    bool Test(Node* n)
    {
      //if( _net->getTriangles(n) == 2 || _net->getTriangles(n) == 0) {
      if( _net->getTriangles(n) == 2 ) {
        return true;
      }
      return false;
    }
	
};

int main(int argc, char* argv[])
{

  NetworkFactory nf;
  Network* net = nf.create(std::cin);
  NodeIterator* ni = net->getNodeIterator();
  NINBIterator fi(ni, net, (NodeIntMember)&Network::getTriangles, 2, false, false);
  while(fi.moveNext()) {
    Node* n = fi.current();
    std::cout << "Node: " << n->toString() << " has: " << net->getTriangles(n)
	    << " triangles" << std::endl;
  }
  ni->reset();
  NodeIterator* ni2 = ni->clone();
  //Must clone because the filter will delete the iterator when we are done
  std::cout << "Round 2" << std::endl;
  Fittest fittest(net);
  FilteredIterator<Fittest,Node*> fi2(ni2, &fittest, &Fittest::Test);
  while( fi2.moveNext() ) {
    Node* n = fi2.current();
    std::cout << "Node: " << n->toString() << " has: " << net->getTriangles(n)
	    << " triangles" << std::endl;
  }
}
