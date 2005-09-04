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

#include "expprobabilityfunction.h"

using namespace Starsky;

ExpProbabilityFunction::ExpProbabilityFunction(double base,
		                                         int min,
							 int max) : _base(base),
                                                                    _min(min),
								    _max(max)
{
  _coeff = (1.0 - _base)/( pow(_base, _min) - pow(_base, (_max + 1)));
  _coeff2 = (1.0 - _base) / (pow(_base, _min) * _coeff);
  _coeff3 = 1.0/log(_base);
}

double ExpProbabilityFunction::getProbabilityOf(int deg) const {
    if( (_min <= deg) && (deg <= _max) ) {
      return _coeff * pow(_base, deg);
    }
    return 0.0;
}
int ExpProbabilityFunction::getRandomDegree(double prob)  {
    return _min + (int)floor( _coeff3 * log(1.0 - _coeff2 * prob) );
}
