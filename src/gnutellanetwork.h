#ifndef starsky__gnutellanetwork
#define starsky__gnutellanetwork

#include "network.h"
#include "superstring.h"
#include "gnutellanode.h"

#include <fstream>
#include <vector>
#include <map>

namespace Starsky {
/**
 * reads graph structure from Gnutella crawl data.
 */
class GnutellaNetwork : public Network {

    public:
        GnutellaNetwork(const std::string& filename,
			const std::string& file_type = "limecrawler");
        GnutellaNode* getNodeByAddress(const std::string& address) const;
	
    protected:
	void readCrawlerFile(const std::string& file);
	void readRipeanuFile(const std::string& file);
	
	std::map<std::string, GnutellaNode*> _add_node_map;
};
	
}

#endif
