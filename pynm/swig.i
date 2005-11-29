%module pynetmod
%{
#include "../src/iterator.h"
#include "../src/node.h"
#include "../src/edge.h"
#include "../src/namednode.h"
#include "../src/nodefactory.h"
#include "../src/edgefactory.h"
#include "../src/networkfactory.h"
#include "../src/network.h"
#include "../src/random.h"
#include "../src/ran1random.h"
#include "../src/networkfactory.h"
#include "../src/degreelawnetfac.h"
#include "../src/discreterandvar.h"
#include "../src/powerlawdrv.h"
#include "../src/intstats.h"
#include "../src/inetworkpartitioner.h"
#include "../src/clusterpart.h"
#include "../src/componentpart.h"
#include "../src/agglompart.h"
#include "../src/newmancom.h"
#include <iostream>
#include <fstream>
%}

//Here are some standard C++ things we need
%include "std_string.i"
%include "std_iostream.i"

//This helps us get the memory management right
%newobject *::clone;

%include "../src/node.h"
%extend Starsky::Node {
  %pythoncode {
    def __str__(self):
      return self.toString()
  }
}
%include "../src/edge.h"
%extend Starsky::Edge {
  %pythoncode {
    def __str__(self):
      return self.toString()
  }
}
%include "../src/iterator.h"

%template(NodeIterator) Starsky::Iterator<Starsky::Node*>;
%template(EdgeIterator) Starsky::Iterator<Starsky::Edge*>;
%template(StringIterator) Starsky::Iterator<std::string>;

//This is so we can do: for n in net.getNodeIterator: in Python
%extend Starsky::Iterator<Starsky::Node*> {
  %pythoncode {
    def __iter__(self):
     while self.moveNext():
       yield self.current();
  }
}

//This is so we can do: for n in net.getEdgeIterator: in Python
%extend Starsky::Iterator<Starsky::Edge*> {
  %pythoncode {
    def __iter__(self):
     while self.moveNext():
       yield self.current();
  }
}

//This is so we can do: for n in net.getEdgeIterator: in Python
%extend Starsky::Iterator<std::string> {
  %pythoncode {
    def __iter__(self):
     while self.moveNext():
       yield self.current();
  }
}

%include "../src/inetworkmonitor.h"

//We should not be using the below in general (currently)
%rename(add_pe) Starsky::Network::add(Edge*);
//These methods allocate memory, so we need to let SWIG know
%newobject Starsky::Network::getEdgeIterator();
%newobject Starsky::Network::getEdgeIterator(Node*);
%newobject Starsky::Network::getNodeIterator();
%newobject Starsky::Network::getNeighborIterator(Node*);
%newobject Starsky::Network::getNeighbors(Node*);
%newobject Starsky::Network::getNIMembers();
%newobject Starsky::Network::getNDMembers();
%newobject Starsky::Network::getEIMembers();
%newobject Starsky::Network::getEDMembers();
%include "../src/network.h"

%extend Starsky::Network {
  void printToFile(char* filename) {
    std::ofstream output(filename);
    self->printTo(output);
  }
}

%include "../src/random.h"
%include "../src/ran1random.h"

%include "../src/nodefactory.h"
%include "../src/edgefactory.h"
%newobject Starsky::NetworkFactory::create();
%newobject Starsky::NetworkFactory::create(std::istream&);
%newobject Starsky::NetworkFactory::create(char*);
%include "../src/networkfactory.h"
%extend Starsky::NetworkFactory {
  Network* create(char* filename) {
    std::ifstream input(filename);
    return self->create(input);
  }
}


%include "../src/discreterandvar.h"
%include "../src/powerlawdrv.h"

%include "../src/degreelawnetfac.h"

%include "std_pair.i"
%include "std_map.i"
%template(pairii) std::pair<int, int>;
%template(mapii) std::map<int,int>;

//IntStats uses some output parameters, here is how we do it:
%include "typemaps.i"
%apply double &OUTPUT { double& h1, double& h2, double& h3 };
%rename(collectN) Starsky::IntStats::collect(const Network* net, NI mem);
%rename(collectE) Starsky::IntStats::collect(const Network* net, EI mem);

namespace Starsky {

class IntStats {
 public:
  IntStats();
  IntStats(bool keep_dist);
  IntStats(bool keep_d, bool keep_mx);
  IntStats(bool keep_d, bool keep_mx, bool keep_min);
  double collectN(const Network* net, std::string m);
  double collectN(const Network* net, std::string m, NodeIterator* ni);
  void collectByEdge(const Network* net, std::string m);
  void collectByEdge(const Network* net, std::string m, EdgeIterator* ei);
  double getAverage() const;
  int getCount() const;
  const std::map<int, int>& getDist() const;
  double getEntropy() const;
  int getMax() const;
  int getMin() const;
  double getEdgeCorrelation() const;
  //let swig know these are output parameters:
  void getEdgeEntropy(double& h1, double& h2, double& h3) const;
  double getEdgeMutualInfo() const;
  double getMoment2() const;
  double getMoment(double m) const;
  double getVariance() const;
};

}
%include "std_vector.i"
%template(vectornet) std::vector<Starsky::Network*>;

//Here are the community/clustering algorithms:
%include "../src/inetworkpartitioner.h"
%include "../src/clusterpart.h"
%include "../src/componentpart.h"
%include "../src/agglompart.h"
%include "../src/newmancom.h"
