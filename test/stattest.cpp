#include <netmodeler.h>
#include <iostream>
#include <memory>

using namespace Starsky;

int main(int argc, char* argv[])
{

  int size = atoi(argv[1]);
  Ran1Random r(-1);
  //PowerLawDRV pl(r, -2.0,2,(int)pow((double)size,0.5));
  //ConstDRV pl(4);
  UniformDRV pl(r, 2, 3);
  std::auto_ptr<NetworkFactory> nf( new DegreeLawNetFac(size, pl, r, false) );
  std::auto_ptr<Network> net( nf->create() );


  NodeIntStats ns(true);
  //ns.collect(net.get(), &Network::getTriangles);
  ns.collect(net.get(), &Network::getDegree);
  //ns.collect(net.get(), &Network::getAssociatedNumber);
  //ns.collectByEdge(net.get(), &Network::getTriangles);
  ns.collectByEdge(net.get(), &Network::getDegree);
  double h1, h2, h3;
  ns.getEdgeEntropy(h1, h2, h3);
  std::cout << "h1: " << h1 << " h2: " << h2 << " h3: " << h3 << std::endl;
  std::cout << "collected" << std::endl;
  std::cout << "max: " << ns.getMax() << std::endl
       << "min: " << ns.getMin() << std::endl
       << "ave: " << ns.getAverage() << std::endl
       << "ave2: " << ns.getMoment2() << std::endl
       << "entropy: " << ns.getEntropy() << std::endl
       << "newmi: " << ns.getEdgeMutualInfo() << std::endl
       << "cor: " << ns.getEdgeCorrelation() << std::endl;
   
}
