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

#ifndef starsky__mrucache_h
#define starsky__mrucache_h

#include <set>
#include <map>
#include "cache.h"

namespace Starsky {

/**
 * MRU (most recently used) cache, every item in the cache has a time stamp
 * Whenever an item is accessed by get() or is re-inserted, the time stamp is 
 * re-stamped to the current time of the get() or insert() function.
 */
class MruCache : public Cache{

    public:

	virtual ~MruCache() { };

	MruCache() { max_size = 500;}

	MruCache(int size) { max_size = size;}

	/**
	 * @param key the key of the item that we want to get
	 * @new_time the current time of the get() operation
	 * @return value of the key
	 */
	virtual value get(key k, double new_time);
	/**
	 * @param key the key of the item that we want to insert
	 * @param v the value of the item that we want to insert
	 * @new_time the current time of the insert() operation	 
	 */
	virtual void insert(key k, value v, double new_time);

    protected:
	///the maximum number of items in the cache
	int max_size;

};
	
}

#endif

