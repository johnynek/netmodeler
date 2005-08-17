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

#ifndef starsky__cache_h
#define starsky__cache_h

#include <set>
#include <map>
#include "contentnode.h"

namespace Starsky {

/**
 * abstract class for cache
 */
//template <class key, class value>
class Cache {

    public:
	virtual ~Cache() { };

	typedef ContentNode* key;  //the key is a ContentNode

	typedef std::string value;  //the value is an int

	/**
	 * @param the key of the item
	 * @return true if the key is in the cache, false otherwise
	 */
	virtual bool contains(key k);

	/**
	 * @param the key of the item
	 * @return true if the time_to_key and key_to_time maps are consistent with each other
	 */
	virtual bool consistent(key k);

	virtual void clear();

	virtual void clearRemovedKeySet() { removed_key_set.clear();}

	virtual void clearNewInsertKeySet() { new_insert_key_set.clear();}

	/**
	 * @param key the key of the item that we want to get
	 * @new_time the current time of the get() operation
	 * @return value of the key
	 */
	virtual value get(key k, double new_time)= 0 ;
	
	/**
	 * @return the set of keys
	 */
	virtual std::set<key> getKeySet();
	/**
	 * @return the set of keys that were removed during a simulation cycle
	 */
	virtual std::set<key> getRemovedKeySet() {return removed_key_set;}
	/**
	 * @return the set of keys that were removed during a simulation cycle
	 */
	virtual std::set<key> getNewInsertKeySet() {return new_insert_key_set;}
	/**
	 * @param key the key of the item that we want to insert
	 * @param v the value of the item that we want to insert
	 * @new_time the current time of the insert() operation	 
	 */
	virtual void insert(key k, value v, double new_time) = 0;

	/**
	 * prints the entire cache displaying key and value
	 */
	virtual void print();

	/**
	 * prints the key to time map
	 */
	virtual void printkeytime();

	/**
	 * prints the time to key multimap
	 */
	virtual void printtimekey();

	/**
	 * @param k the key of the item that we want to remove
	 * @param v the value of the item that we want to remove
	 */
	virtual void remove(key k, value v) ;

	/**
	 * @return the size of the cache
	 */
	virtual int size();

	/**
	 * @param key the key of the item that we want to update
	 * @new_time the current time of the get() operation
	 * This function simply changes the time stamp of the key k
	 * it will change the time_to_key and key_to_time maps
	 */
	virtual void update(key k, double new_time);

    protected:

	std::multimap<double, key> time_to_key;
	std::map<key, double> key_to_time;
	std::map<key, value> key_to_value;

	///the removed key set stored all the keys that were removed in a given run
	///of simulation.  so, when nodes do the random walk around and update the
	///cache on other nodes, this set of removed keys must be removed first
	std::set<key> removed_key_set;

	///the new insert key set stored all the keys that were newly inserted in a given run
	///of simulation.  so, when nodes do the random walk around and update the
	///cache on other nodes, this set of newly-inserted keys will be inserted
	std::set<key> new_insert_key_set;

};
	
}

#endif
