/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

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
