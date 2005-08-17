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

#ifndef starsky__exppreference
#define starsky__exppreference


#include "degreepreferencefunction.h"
#include <cmath>

namespace Starsky {
 /**
  * a preferential attachment function of the form:
  *  exp { \beta k } + \gamma
  * where k is the degree
  */

class ExpPreference : public DegreePreferenceFunction {

    public:
        ExpPreference(double beta = 1.0, double p0 = 0.0);
	double evaluate(int degree) const;
    protected:
	double _beta;
	double _p0;
	
};
	
}

#endif
