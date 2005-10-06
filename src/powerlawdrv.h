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

#ifndef starsky__powerlawdrv
#define starsky__powerlawdrv

#include "discreterandvar.h"
#include "random.h"
#include <cmath>
#include <map>

namespace Starsky {

/**
 * Degree distribution function for a power-law with a given
 * exponent, minimum degree, and maximum degree.
 */

class PowerLawDRV : public DiscreteRandVar {

    public:
	/**
	 * @param r the RNG to use
	 * @param exponent the exponent of the power law.
	 * @param min_deg the minimum degree allowed
	 * @param max_deg the maximum degree allowed (defaults to (2^31-1), which is ~2*10^9)
	 */
        PowerLawDRV(Random& r,
			            double exponent = -2.0,
				    int min_deg = 1,
				    int max_deg = 0x7FFFFFFF);
	
	double getProbabilityOf(int deg) const;
	int sample();
	int getMin() const;
	int getMax() const;
    protected:
	Random& _rand;
	double _expon;
	double _coeff;
	int _min;
	int _max;
	//The below is used to do efficient look up of the cdf of a given degree
	std::map<double, int> _cdf_to_idx;
};
	
}

#endif
