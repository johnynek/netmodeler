#include "debianpackagenode.h"

using namespace Starsky;
using namespace std;

DebianPackageNode::DebianPackageNode(const string& name, const string& version, int size)
: DebianNode(name), _version(version), _size(size) {

}

int DebianPackageNode::getSize() const { return _size; }
