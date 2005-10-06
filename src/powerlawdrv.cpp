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

#include "powerlawdrv.h"

#include <iostream>

using namespace Starsky;
using namespace std;

PowerLawDRV::PowerLawDRV(Random& r, double expon,
		                                         int min,
							 int max) : _rand(r),
                                                                    _expon(expon),
                                                                    _min(min),
								    _max(max)
{
    int i;
    int steps = 0;
    double sum = 0.0;
    for(i = _min; i < _max; i++) {
	double this_val = pow((double)i, _expon);
        sum += this_val;
	if( (steps % 10) == 0 ) {
          //Every hundred steps add value:
	  _cdf_to_idx[ sum ] = i;
	}
    }
    _coeff = 1.0/sum;
}

double PowerLawDRV::getProbabilityOf(int deg) const {
    if( (_min <= deg) && (deg <= _max) ) {
      return _coeff * pow((double)deg, _expon);
    }
    return 0.0;
}
int PowerLawDRV::sample()  {
    double prob = _rand.getDouble01();
    double p_over_c = prob / _coeff;
    map<double, int>::const_iterator cit = _cdf_to_idx.upper_bound(p_over_c);
    //This is a new upper bound:
    if( cit != _cdf_to_idx.begin() ) {
      //Back up one...
      cit--;
    }
    double sum = cit->first;
    int i = cit->second;
    int steps = 0;
    while( (sum < p_over_c) && ( i < _max) ) {
	//std::cout << "i: " << i << " sum: " << sum << std::endl;
	++i;
        sum += pow((double)i, _expon);
	steps++;
    }
    if( steps > 4 ) {
       //Now insert this value so we can look up faster:
      _cdf_to_idx[ sum ] = i;
    }
#if 0
    std::cout << "p_over_c: " << p_over_c << " sum: " << sum
	      << " idx: " << i << std::endl;
#endif
    return i;
}

int PowerLawDRV::getMin() const {
    return _min;
}

int PowerLawDRV::getMax() const {
    return _max;
}

