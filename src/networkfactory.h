#ifndef starsky__networkfactory_h
#define starsky__networkfactory_h

#include <network.h>
#include <nodefactory.h>
#include <edgefactory.h>

namespace Starsky {

/**
 * Base class for making Networks.
 * This can read from different file types,
 * or make different networks according to
 * some algorithm
 */
class NetworkFactory {

  public:
    NetworkFactory();
    /**
     * These pointers are deleted when this NetworkFactory
     * is deleted
     */
    NetworkFactory(NodeFactory* nf, EdgeFactory* ef);
    virtual ~NetworkFactory();
   
    virtual Network* create() { return 0; }
    /**
     * Read from a file.
     */
    virtual Network* create(std::istream& input);

    virtual NodeFactory* getNodeFactory() { return _nf; }
    virtual EdgeFactory* getEdgeFactory() { return _ef; }
  protected:
    NodeFactory* _nf;
    EdgeFactory* _ef;
    
};

}

#endif
