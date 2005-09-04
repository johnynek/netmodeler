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

#ifndef starsky__expprobabilityfunction
#define starsky__expprobabilityfunction

#include "degreeprobabilityfunction.h"
#include <cmath>
#include <vector>

namespace Starsky {

/**
 * Degree distribution function for a exponential probability
 * distribution.
 */

class ExpProbabilityFunction : public DegreeProbabilityFunction {

    public:
	/**
	 * @param base that is exponentiated
	 * @param min_deg the minimum degree allowed
	 * @param max_deg the maximum degree allowed (defaults to (2^31-2), which is ~2*10^9)
	 * p_k = base^(-k) for all k between min_deg and max_deg
	 */
        ExpProbabilityFunction(double base = 0.5,
				    int min_deg = 1,
				    int max_deg = 0x7FFFFFFE);
	
	double getProbabilityOf(int deg) const;
	int getRandomDegree(double prob);
	int minDegree() const { return _min; }
	int maxDegree() const { return _max; }
    protected:
	double _base;
	double _coeff;
	double _coeff2;
	double _coeff3;
	int _min;
	int _max;
};
	
}

#endif
