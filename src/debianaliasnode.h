#ifndef starsky__debianaliasnode_h
#define starsky__debianaliasnode_h

#include "debiannode.h"

namespace Starsky {

/**
 * this class represents the aliases that debian uses for "Provides".
 * Any Package that this file links to, is sufficient to provide for the
 * dependencies of the alias.
 */
	
class DebianAliasNode : public DebianNode {
  public: 
    DebianAliasNode(const std::string& name) : DebianNode(name) { }
};
	
}

#endif
