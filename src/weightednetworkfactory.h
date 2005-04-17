#ifndef starsky__weightednetworkfactory_h
#define starsky__weightednetworkfactory_h

#include <networkfactory.h>
#include <weightededgefactory.h>

namespace Starsky {

/**
 * Base class for making Networks.
 * This can read from different file types,
 * or make different networks according to
 * some algorithm
 */
class WeightedNetworkFactory : public NetworkFactory {

  public:
    WeightedNetworkFactory();
    /**
     * These pointers are deleted when this NetworkFactory
     * is deleted
     */
    WeightedNetworkFactory(NodeFactory* nf, EdgeFactory* ef);
   
    virtual Network* create() { return 0; }
    /**
     * Read from a file
     */
    virtual Network* create(std::istream& input);
  protected:
    NodeFactory* _nf;
    EdgeFactory* _ef;
    
};

}

#endif
