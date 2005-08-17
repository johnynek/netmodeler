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

#ifndef starsky__inetworkpartioner_h
#define starsky__inetworkpartioner_h

#include <set>
#include <map>
#include <network.h>

namespace Starsky {

   /**
    * This function deletes the result of a previous
    * partition.  This should be called when the
    * data is no longer used
    */
class INetworkPartitioner {

  public:
   virtual void deletePartition(std::set<Network*>*  part);
  /**
   * Interface which Community finding algorithms
   * can subclass
   */
   double modularityOf(std::set<Network*>* partition, const Network& orig);
   /**
    * The caller of this function should delete this memory
    * when done (Network* and set<>*)
    */
   virtual std::set<Network*>* partition(const Network& input) = 0;
   
   /**
    * Uses the Frobenius norm to compute |A - B|, |A|, and |B| for
    * the communities given.  The partition matrix is an N x N matrix
    * with 1 at (i,j) if node i and node j are in the same partition.
    * The diagonal is zero for our calculation of |A|.  e.g. the network
    * with each node in its own partition has |A| = 0.
    *
    * If one set of nodes is larger than the other, the matrix is 0
    * for any missing nodes.
    */
   virtual long distance(std::set<Network*>* A, std::set<Network*>* B,
		        long& norm_a, long& norm_b);
};
	
}

#endif
