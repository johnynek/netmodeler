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

