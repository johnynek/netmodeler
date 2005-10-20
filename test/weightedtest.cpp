#include <netmodeler.h>
#include <iostream>

using namespace Starsky;

#define DEBUG(msg) std::cout << msg << std::endl;

int main(int argc, char* argv[])
{
  //NetworkFactory* nf = new WeightedNetworkFactory();
  NetworkFactory* nf = new NetworkFactory();
  DEBUG("made nf")
  Network* net = nf->create(std::cin);
  DEBUG("made net")
  net->printTo(std::cout);
}
