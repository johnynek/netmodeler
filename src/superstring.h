#ifndef starsky__superstring_h
#define starsky__superstring_h

#include <string>
#include <vector>

namespace Starsky {

class SuperString : public std::string {

    public:
	SuperString();
        SuperString(const std::string& astring);
        
	std::vector<SuperString> split(const std::string& delim) const;
};
	
}

#endif
