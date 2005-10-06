/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
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

#include "uniformdrv.h"

using namespace Starsky;

UniformDRV::UniformDRV(Random& r, int min, int max) : _rand(r), _min(min), _max(max)
{
  if ( _max < _min ) {
    throw std::exception();
  }
}


double UniformDRV::getProbabilityOf(int degree) const
{
  if( ( _min <= degree ) && ( degree <= _max ) ) {
    int total = _max - _min + 1;
    return 1.0/(double)(total);  
  }
  else {
    return 0.0;
  }
}

int UniformDRV::sample()
{
  return _rand.getInt(_max, _min);
}

int UniformDRV::getMin() const { return _min; }
int UniformDRV::getMax() const { return _max; }
