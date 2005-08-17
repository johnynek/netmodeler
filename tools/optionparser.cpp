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

#include "optionparser.h"

#include <iostream>
#include <sstream>

using namespace std;
using namespace Starsky;

map<string, string> OptionParser::getOpts(int argc, char* argv[])
{
  vector<string> empty;
  return getOpts(argc, argv, empty);
}

map<string, string> OptionParser::getOpts(int argc, char* argv[],
		                          vector<string>& pos_keys)
{
  vector<string> empty;
  return getOpts(argc,  argv, pos_keys, empty);
}

map<string, string> OptionParser::getOpts(int argc, char* argv[],
		                          vector<string>& req_keys,
					  vector<string>& pos_keys)
{
  map<string, string> result;
  int pos = 0;
  for(int i = 1; i < argc; i++) {
    if( argv[i][0] == '-' && argv[i][1] == '-' ) {
      //This is an option
      char* this_arg = argv[i] + 2;
      //Skip the --
      string arg = string(this_arg);
      //Find the '='
      int eqpos = arg.find('=');
      if( eqpos != string::npos ) {
        string key = arg.substr(0, eqpos);
	string value = arg.substr(eqpos + 1, string::npos);
	result[key] = value;
	cout << key << " -> " << value << endl;
      }
      else {
        result[arg] = "";
      }
    }
    else {
    //This is a positional option;
      if( pos < req_keys.size() ) {
        result[ req_keys[pos] ] = argv[i];
        pos++;
      }
      else if( pos < ( pos_keys.size() + req_keys.size() ) ) {
        result[ pos_keys[ pos - req_keys.size() ] ] = argv[i];
	pos++;
      }
      else {
        //We are loosing this option:
	cerr << "Unknown option: " << argv[i] << endl;
	throw exception();
      }
    }
  }
  for( int i = 0; i < req_keys.size(); i++) {
    if( result.find( req_keys[i] ) == result.end() ) {
     throw OptionException( req_keys[i] );  
    }
  }
  return result;
}


string OptionParser::getUsageString(std::vector<std::string>& req_keys,
                               std::vector<std::string>& opts)
{
  stringstream ss;
  for(int i = 0; i < req_keys.size(); i++)
    ss << " " << req_keys[i] << " ";
  for(int i = 0; i < opts.size(); i++)
    ss << " [" << opts[i] << "] ";
  return ss.str();
}
