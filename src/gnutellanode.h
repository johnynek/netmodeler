#ifndef starsky__gnutellanode
#define starsky__gnutellanode

#include <string>
#include "node.h"

namespace Starsky {

class GnutellaNode : public Node {

    public:
        GnutellaNode(const std::string& address,
		     const std::string& vendor = "",
		     bool is_ultra = false,
		     int distance = 0);
	//Returns the address:ip:vendor
	std::string toString() const;
    protected:

	std::string _address;
	std::string _vendor;
	bool _is_ultrapeer;
	int _distance;
};
	
}

#endif
