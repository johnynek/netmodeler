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
  cnt_ptr<Network> net( nf.create(std::cin) );
  NodeIterator* ni = net->getNodeIterator();
  NINBIterator fi(ni, net.get(), (NodeIntMember)&Network::getTriangles, 2, false, true);
  while(fi.moveNext()) {
    Node* n = fi.current();
    std::cout << "Node: " << n->toString() << " has: " << net->getTriangles(n)
	    << " triangles" << std::endl;
  }
  ni->reset();
  NodeIterator* ni2 = ni->clone();
  //Must clone because the filter will delete the iterator when we are done
  std::cout << "Round 2" << std::endl;
  Fittest fittest(net.get());
  ClassFilterator<Fittest,Node*> fi2(ni2, &fittest, &Fittest::Test);
  while( fi2.moveNext() ) {
    Node* n = fi2.current();
    std::cout << "Node: " << n->toString() << " has: " << net->getTriangles(n)
	    << " triangles" << std::endl;
  }

  //Stl test:
  fi2.reset();
  std::list<Node*> node_list;
  fi2.pushInto(node_list);
  StlIterator<std::list, Node*> stlit(node_list);
  while( stlit.moveNext() ) {
    std::cout << stlit.current()->toString() << std::endl;
  }
  std::set<Node*> node_set;
  stlit.reset();
  stlit.insertInto(node_set);
  Iterator<Node*>* stlit2 = new StlIterator<std::list, Node*>(node_list);
  while( stlit2->moveNext() ) {
    std::cout << stlit2->current()->toString() << std::endl;
  }
  delete stlit2;
  std::cout << "StlPIterator test:" << std::endl;
  std::set<Node*>* ns = new std::set<Node*>( node_set );
  Iterator<Node*>* pstl1 = new StlPIterator<std::set, Node*>(ns);
  while( pstl1->moveNext() ) {
    std::cout << pstl1->current()->toString() << std::endl;
  }
  std::cout << "Now copy" << std::endl;
  Iterator<Node*>* pstl2 = pstl1->clone();
  pstl2->reset();
  std::cout << "Node delete the first" << std::endl;
  delete pstl1;
  while( pstl2->moveNext() ) {
    std::cout << pstl2->current()->toString() << std::endl;
  }
  delete pstl2;
  //This memory should be deleted by now, so this might (should) segfault
  std::cout << "Node size is: " << std::endl <<  ns->size() << std::endl;
}
