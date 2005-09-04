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

#ifndef starsky__optionparser_h
#define starsky__optionparser_h

#include <map>
#include <string>
#include <vector>
#include <exception>
#include <iostream>

namespace Starsky {

class OptionException : public std::exception {
  private:
    std::string _what;
  public:
    OptionException(const std::string& what)  { _what = what; };
    virtual ~OptionException() throw() { };

    virtual const char* what() const throw() { return _what.c_str(); };
};
	
/**
 * A simple option parser for command line arguments.
 * 
 *
 * For now, it just accepts --option=value arguments
 */
	
class OptionParser {

  public:
    /**
     * Constructor for using the "non-static" mode of this class.
     */
    OptionParser(std::vector<std::string>& req_keys,
                    std::vector<std::string>& opts);
   
    /**
     * return the boolean value of the option opt if it
     * was given, else return the default_value.
     * 1, TRUE, True, true count as true, all else false (if given).
     */
    bool getBoolOpt(std::string opt, bool default_value);
    double getDoubleOpt(std::string opt, double default_value);
    /**
     * Returns the value of the option if it was given, if not
     * it returns the default value
     */
    int getIntOpt(std::string name, int default_value);
    
    static std::map<std::string, std::string> getOpts(int argc, char* argv[]);
    /**
     * @param pos, this contains the name of the i^th unnamed option_key
     */
    static std::map<std::string, std::string> getOpts(int argc,
		                               char* argv[],
					       std::vector<std::string>& pos);
    /**
     * @param req_keys the keys we are required to have, else we throw exception
     * @param opts options that are not required.
     */
    static std::map<std::string, std::string> getOpts(int argc,
		                               char* argv[],
					       std::vector<std::string>& req_keys,
					       std::vector<std::string>& opts);
    /**
     * Returns a string that explains the usage
     */
    static std::string getUsageString(std::vector<std::string>& req_keys,
			       std::vector<std::string>& opts);
    std::string getUsageString();
    /**
     * After initializing the object, you call this method
     * to parse the options.
     * @throws OptionException if there are missing required options.
     */
    void parse(int argv, char* argv[]);
  protected:
    std::map<std::string, std::string> _options;
    std::vector<std::string> _reqs;
    std::vector<std::string> _opt; ///Optional options.
};

}

#endif
