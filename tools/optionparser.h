#ifndef starsky__optionparser_h
#define starsky__optionparser_h

#include <map>
#include <string>
#include <vector>
#include <exception>
#include <iostream>

namespace Starsky {
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
};

}

#endif
