#include "mrucache.h"
#include <iostream>
#include <map>
#include <stdexcept>

//#define DEBUG

using namespace std;
using namespace Starsky;

Cache::value MruCache::get(key k, double new_time){
  if(Cache::contains(k) ){
    Cache::update(k, new_time);
    return key_to_value[k];    
  }
  else{
     //throw an exception
    throw invalid_argument(string("Error: key not in cache! from mrucache.cpp get()"));
  }
}

void MruCache::insert(key k, value v, double new_time){
  if(!contains(k) ){//inserting new item
    if( size() < max_size){ //cache is not full yet
#ifdef DEBUG
      // cout << "got here: mru 1." << endl;
#endif
      key_to_value[k] = v;
      key_to_time[k] = new_time;
      std::pair<double, key> p(new_time, k);
      time_to_key.insert(p);
      new_insert_key_set.insert(k);
    }
    else if( size() == max_size){ //cache is full
#ifdef DEBUG
      cout << "Some cache is full" << endl;
#endif
      //figure out which entry is the oldest and remove it then insert again
      std::map<double, key>::const_iterator m_it;
      m_it = time_to_key.begin();  //this gets the oldest entry (with earliest time stamp)
      key oldest_key = (*m_it).second;

      key_to_time.erase(oldest_key);
      key_to_value.erase(oldest_key); 
      time_to_key.erase(time_to_key.begin());

      ///insert the key to be removed in the removed_key_set
      removed_key_set.insert(oldest_key);

      insert(k, v, new_time);
    }
    else{
#ifdef DEBUG
    cout << "got here: mru 3." << endl;
#endif
      //throw an exception
      throw overflow_error(string("Error: Cache overflow!  mrucache.cpp insert()"));
    }
  }
  else{//item already exists, simply change the time stamp!  
#ifdef DEBUG
    // cout << "got here: mru 4." << endl;
#endif  
    Cache::update(k, new_time);
  }

}
