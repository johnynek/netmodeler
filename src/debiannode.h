#ifndef starsky__debiannode_h
#define starsky__debiannode_h

#include "node.h"
#include <string>
#include "superstring.h"

namespace Starsky {

class DebianNode : public Node {

 public:
    DebianNode(const std::string& name);
    std::string getName() const;
    std::string toString() const;
    bool isLib() const;
    virtual int getSize() const;
 protected:
    std::string _package;
    bool _is_lib;
};
	
}

#endif

