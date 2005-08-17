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

#include "random.h"

using namespace Starsky;

bool Random::getBool() {
  return (getDouble01() < _true_bias);
}

int Random::getInt() {
  return (int)(( (_imax + 1) - _imin) * getDouble01()) + _imin;
}

int Random::getInt(int high, int low) {
  if ( low > high)
    throw std::invalid_argument("RNG args out of order");
  return (int)(( (high + 1) - low) * getDouble01()) + low;
}

double Random::getDouble() {
  return (_dmax - _dmin) * getDouble01() + _dmin;
}

double Random::getDouble(double high, double low) {
  if ( low > high)
    throw std::invalid_argument("RNG args out of order");
  return (high - low) * getDouble01() + low;
}

void Random::setDoubleRange(double high, double low) {
  if ( low > high)
    throw std::invalid_argument("RNG args out of order");
    _dmax = high;
    _dmin = low;
}

void Random::setIntRange(int high, int low) {
  if ( low > high)
    throw std::invalid_argument("RNG args out of order");
	_imax = high;
  _imin = low;
}

void Random::setBoolTrueBias(double p) {
  _true_bias = p;
}
