#ifndef starsky__debianpackagenode_h
#define starsky__debianpackagenode_h

/**
 * Represents actual packages which can be installed.  The out degrees are those
 * packages they depend upon, and the in degrees are those packages that depend on
 * them
 */

#include "debiannode.h"
#include <string>

namespace Starsky {

class DebianPackageNode : public DebianNode {

  public: 
    DebianPackageNode(const std::string& name, const std::string& version, int size);
    std::string getVersion() const;
    int getSize() const;
  protected:
    std::string _version;
    int _size;
};
	
}

#endif
