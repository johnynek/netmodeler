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

#ifndef starsky__degreeprobabilityfunction
#define starsky__degreeprobabilityfunction

namespace Starsky {

  /**
   * Abstract class for defining degree probability functions
   * for use in producing random networks with given degree
   * distributions.
   */
	
class DegreeProbabilityFunction {

    public:
	/**
	 * @return the probability that a node has a given degree
	 */
        virtual double getProbabilityOf(int degree) const = 0;
	/**
	 * @param prob a random cdf value (0.0,1.0)
	 * @return the smallest degree with cdf > prob
	 */
	virtual int getRandomDegree(double prob) = 0;
	/**
	 * @return minimum degree with p > 0
	 */
	virtual int minDegree() const = 0;
	/**
	 * @return maximum degree with p > 0
	 */
	virtual int maxDegree() const = 0;
	
};
	
}

#endif
