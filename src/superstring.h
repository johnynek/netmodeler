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

#ifndef starsky__superstring_h
#define starsky__superstring_h

#include <string>
#include <vector>

namespace Starsky {

/**
 * This class is to add methods that most modern languages have
 * to the string class.  The idea is to make it easier to parse
 * files with a more poweful string class
 */
class SuperString : public std::string {

    public:
	/**
	 * Make an empty Superstring
	 */
	SuperString();
	/**
	 * Make a new superstring from the given string
	 */
        SuperString(const std::string& astring);
        
	/**
	 * Split the current string around a given delimeter.
	 * @param delim the delimeter around which to split the string
	 * @return a vector of SuperString.
	 */
	std::vector<SuperString> split(const std::string& delim) const;
};
	
}

#endif
