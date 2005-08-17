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

#ifndef starsky__gnutellanode
#define starsky__gnutellanode

#include <string>
#include "node.h"

namespace Starsky {

class GnutellaNode : public Node {

    public:
        GnutellaNode(const std::string& address,
		     const std::string& vendor = "",
		     bool is_ultra = false,
		     int distance = 0);
	//Returns the address:ip:vendor
	std::string toString() const;
    protected:

	std::string _address;
	std::string _vendor;
	bool _is_ultrapeer;
	int _distance;
};
	
}

#endif
