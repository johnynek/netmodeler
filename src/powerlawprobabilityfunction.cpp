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

#include "powerlawprobabilityfunction.h"

using namespace Starsky;

PowerLawProbabilityFunction::PowerLawProbabilityFunction(double expon,
		                                         int min,
							 int max) : _expon(expon),
                                                                    _min(min),
								    _max(max)
{
    int i;
    double sum = 0.0;
    for(i = _min; i < _max; i++) {
        sum += pow((double)i, _expon);
    }
    _coeff = 1.0/sum;
}

double PowerLawProbabilityFunction::getProbabilityOf(int deg) const {
    if( (_min <= deg) && (deg <= _max) ) {
      return _coeff * pow((double)deg, _expon);
    }
    return 0.0;
}
int PowerLawProbabilityFunction::getRandomDegree(double prob) const {
    double p_over_c = prob / _coeff;
    int i = _min;
    double sum = pow((double)i, _expon);
    while( (sum < p_over_c) && ( i < _max) ) {
	++i;
        sum += pow((double)i, _expon);
    }
    return i;
}

int PowerLawProbabilityFunction::minDegree() const {
    return _min;
}

int PowerLawProbabilityFunction::maxDegree() const {
    return _max;
}

