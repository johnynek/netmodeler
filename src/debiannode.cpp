#include "debiannode.h"
using namespace std;
using namespace Starsky;

DebianNode::DebianNode(const string& package)
: _package(package) {

    //This is really a guess, if it has the string "lib" assume it is a library
    string::size_type lib_pos = 0;
    lib_pos = package.find("lib");
    if( lib_pos != string::npos ) {
        _is_lib = true;
    }
    else {
        _is_lib = false;
    }
}

string DebianNode::getName() const { return _package; }

int DebianNode::getSize() const { return 0; }

bool DebianNode::isLib() const { return _is_lib; } 

string DebianNode::toString() const {
  return _package;
}
