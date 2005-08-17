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

#ifndef starsky__powerlawprobabilityfunction
#define starsky__powerlawprobabilityfunction

#include "degreeprobabilityfunction.h"
#include <cmath>
#include <vector>

namespace Starsky {

/**
 * Degree distribution function for a power-law with a given
 * exponent, minimum degree, and maximum degree.
 */

class PowerLawProbabilityFunction : public DegreeProbabilityFunction {

    public:
	/**
	 * @param exponent the exponent of the power law.
	 * @param min_deg the minimum degree allowed
	 * @param max_deg the maximum degree allowed (defaults to (2^31-1), which is ~2*10^9)
	 */
        PowerLawProbabilityFunction(double exponent = -2.0,
				    int min_deg = 1,
				    int max_deg = 0x7FFFFFFF);
	
	double getProbabilityOf(int deg) const;
	int getRandomDegree(double prob) const;
	int minDegree() const;
	int maxDegree() const;
    protected:
	double _expon;
	double _coeff;
	int _min;
	int _max;
	//The below is used to do efficient look up of the cdf of a given degree
	std::vector<int> _frac_to_deg;
	std::vector<double> _frac_to_cdf;
};
	
}

#endif
