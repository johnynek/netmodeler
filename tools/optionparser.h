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
    std::map<std::string, std::string> getOpts(int argc, char* argv[]);
    /**
     * @param pos, this contains the name of the i^th unnamed option_key
     */
    std::map<std::string, std::string> getOpts(int argc,
		                               char* argv[],
					       std::vector<std::string>& pos);
    /**
     * @param req_keys the keys we are required to have, else we throw exception
     * @param opts options that are not required.
     */
    std::map<std::string, std::string> getOpts(int argc,
		                               char* argv[],
					       std::vector<std::string>& req_keys,
					       std::vector<std::string>& opts);
    /**
     * Returns a string that explains the usage
     */
    std::string getUsageString(std::vector<std::string>& req_keys,
			       std::vector<std::string>& opts);
};

}

#endif
