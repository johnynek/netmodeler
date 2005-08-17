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

#ifndef starsky__magnetmessage
#define starsky__magnetmessage

#include "message.h"
#include "random.h"

namespace Starsky {

/**
 * A message which is passed to a random neighbor with probability
 * (1-p) and to go to the neighbor with maximum degree with
 * probability p.
 */
	
class MagnetMessage : public Message {

    public:
        MagnetMessage(Random& r, double p, int ttl=-1);
	void visit(Node* n, Network& aNet);
	
    protected:
	Random& _rand;
	double _p;
	int _ttl;
};
	
}

#endif
