#include <netmodeler.h>
#include <cstdio>

/**
 * This is a very simple tool.  It applies the TriangleMapper on a network
 * and prints the network back out.
 */

using namespace Starsky;

int main(int argc, char* arvg[]) {

  TriangleMapper tmap;
  NetworkFactory nf;

  Network* net = nf.create(std::cin);
  tmap.map(net);
  net->printTo(std::cout);
}
