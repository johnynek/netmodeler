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
