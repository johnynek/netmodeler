#ifndef starsky__debiandependencynetwork_h
#define starsky__debiandependencynetwork_h

#include "directednetwork.h"
#include "debiannode.h"
#include "debianaliasnode.h"
#include "debianpackagenode.h"
#include "directededge.h"
#include "superstring.h"

#include <string>
#include <map>
#include <fstream>
#include <vector>

namespace Starsky {

class DebianDependencyNetwork : public DirectedNetwork {

    public:
        DebianDependencyNetwork(const std::string& filename );
	DebianNode* getNodeByName(const std::string& package) const;
	double getSizeAssortativity() const;
	double getLibAssortativity() const;
	
    protected:
	void readPackageFile(const std::string& filename);
        static std::vector<SuperString> splitDependencies(const SuperString& depstring);
        std::map<std::string, DebianNode*> name_node_map;
};
	
}

#endif
