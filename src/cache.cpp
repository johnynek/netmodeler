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

#include "cache.h"
#include <iostream>
#include <set>
#include <map>
#include <stdexcept>

//#define DEBUG

using namespace std;
using namespace Starsky;

bool Cache::contains(key k){

  map<key, value>::const_iterator m_it1;
  map<key, double>::const_iterator m_it2;

  m_it1 = key_to_value.find(k);
  m_it2 = key_to_time.find(k);
  
  if(m_it1 != key_to_value.end() && m_it2 != key_to_time.end()){
    return true;
  }
  else{
    return false;
  }	
}

bool Cache::consistent(key k){ //need more work!
  map<key, value>::const_iterator m_it1;
  map<key, double>::const_iterator m_it2;

  m_it1 = key_to_value.find(k);
  m_it2 = key_to_time.find(k);
  bool found = false;
  if(m_it1 != key_to_value.end() && m_it2 != key_to_time.end()){
    double this_time = key_to_time[k];  
    //the following is the tricky part since one time might map to several keys
      std::multimap<double, key>::iterator m_it;
      m_it = time_to_key.find( this_time );
      while( !found && (*m_it).first == this_time && m_it != time_to_key.end() ){
	if( (*m_it).second == k ){
	  found = true;
	  return found;
	}
	m_it++;
      }//end of while loop
  }
  return found;
}

void Cache::clear(){
  time_to_key.clear();
  key_to_time.clear();
  key_to_value.clear();
  removed_key_set.clear();
}

void Cache::print(){
  cout << "Key" << '\t' <<  "Value" << endl;
  map<key, value>::const_iterator m_it;
  for(m_it = key_to_value.begin(); m_it != key_to_value.end(); m_it++){
    cout << (*m_it).first << '\t' << (*m_it).second << endl;
  }
}

set<Cache::key> Cache::getKeySet(){
  set<key> ret;
  map<key, value>::const_iterator m_it;
  for(m_it = key_to_value.begin(); m_it != key_to_value.end(); m_it++){
    ret.insert( (*m_it).first);
  }
  return ret;
}

void Cache::printkeytime(){
  cout << "Key" << '\t' <<  "Time" << endl;
  map<key, double>::const_iterator m_it;
  for(m_it = key_to_time.begin(); m_it != key_to_time.end(); m_it++){
    cout << (*m_it).first << '\t' << (*m_it).second << endl;
  }
}

void Cache::printtimekey(){
  cout << "Time" << '\t' <<  "Key" << endl;
  map<double, key>::const_iterator m_it;
  for(m_it = time_to_key.begin(); m_it != time_to_key.end(); m_it++){
    cout << (*m_it).first << '\t' << (*m_it).second << endl;
  }
}

//template <class key, class value>
void Cache::remove(key k, value v){
  if(Cache::contains(k)){
    double to_remove = key_to_time[k];
    key_to_time.erase(k);
    key_to_value.erase(k);  

    //the following is the tricky part since one time might map to several keys
    int num_occur = time_to_key.count( to_remove);
#ifdef DEBUG
    //cout << "num_occur is: " << num_occur << endl;
#endif
    if(num_occur == 1){
      time_to_key.erase(to_remove);
    }
    else if(num_occur > 1){
      bool found = false;
      std::multimap<double, key>::iterator m_it;
      m_it = time_to_key.find(to_remove);
      while( !found && (*m_it).first == to_remove && m_it != time_to_key.end() ){
	if( (*m_it).second == k ){
	  time_to_key.erase(m_it);
	  found = true;
	  return;
	}
	m_it++;
      }
    }
    else{
      //throw an exception
      throw invalid_argument(string("Error in remove() cache.cpp, inconsistent time_to_key map!"));
    }  
  }
  else{
      //throw an exception
      throw invalid_argument(string("Error in remove() cache.cpp, key does not exist!"));
  }
}

int Cache::size(){
  int num1, num2, num3;
  num1 = key_to_value.size();
  num2 = key_to_time.size();
  num3 = time_to_key.size();
  if(num1 != num2 || num1 != num3 || num2 != num3){
    //throw an exception
    throw logic_error(string("Error: Sizes in cache are NOT consistent! cache.cpp size()"));
  }
  return key_to_value.size();
}
 
void Cache::update(key k, double new_time){
    std::set<key>::const_iterator s_it = removed_key_set.find(k);
    ///make sure that we not updating a removed key!!
    if(s_it == removed_key_set.end()){  
  std::pair<double, key> p(new_time, k);
    time_to_key.insert(p);
   
    double old_time = key_to_time[k];
    key_to_time[k] = new_time;

    //the following is the tricky part since one time might map to several keys
    int num_occur = time_to_key.count( old_time );
#ifdef DEBUG
    cout << "num_occur is:  " << num_occur << " and the new_time is: " << new_time << endl;
#endif
    if(num_occur == 1){
      time_to_key.erase( old_time);      
    }
    else if(num_occur > 1){
      bool found = false;
      std::multimap<double, key>::iterator m_it;
      m_it = time_to_key.find( old_time );
      while( !found && (*m_it).first == old_time && m_it != time_to_key.end() ){
	if( (*m_it).second == k ){
	  time_to_key.erase(m_it);
	  found = true;
	  return;
	}
	m_it++;
      }//end of while loop
      if(!found){
	  throw invalid_argument(string("Error in update() cache.cpp, inconsistent time_to_key map (1)!"));
      }
    }
    else{
      //throw an exception
	throw invalid_argument(string("Error in update() cache.cpp, inconsistent time_to_key map (2)!"));      
    } 

    } 
}
