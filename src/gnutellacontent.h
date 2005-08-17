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

#ifndef starsky__gnutellacontent
#define starsky__gnutellacontent

#include "contentnode.h"
#include <string>
#include <set>

namespace Starsky {

/**
 * Represents content in gnutella content crawls.  Used for looking
 * at Gnutella in the content space.
 */

class GnutellaContent : public ContentNode {

    public:
        GnutellaContent(const std::string& sha1, const std::string& filename, int size);
	/**
	 * Some files in gnutella might have the same sha1 hash and size, but different names.
	 * @param filename the filename to add for this content
	 */
	void addFileName(const std::string& filename);
	
	const std::set<std::string>& getFileNameSet() const;
	int getSize() const;
        const std::string& getSha1() const;

    protected:
	std::string _sha1;
	std::set<std::string> _filename_set;
	int _size;
};

}

#endif
