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

#include "superstring.h"

using namespace Starsky;
using namespace std;

SuperString::SuperString() : string() {
}

SuperString::SuperString(const string& a) : string(a) {
}

vector<SuperString> SuperString::split(const string& delim) const {

    vector<SuperString> return_value;
    string front, back;
    string::size_type last = 0, pos = 0;
    while( (pos < size()) && (pos != string::npos) ) {
        pos = find(delim, last);
	if( pos != string::npos ) {
            return_value.push_back( substr(last, pos - last) );
	}
	else {
            return_value.push_back( substr(last, size() - last) );
	}
	last = pos + delim.size(); //Skip over the delimeter string
    }
    return return_value;
}
