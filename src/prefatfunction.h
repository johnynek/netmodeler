#ifndef starsky__prefatfunction
#define starsky__prefatfunction


#include "network.h"
#include "directednetwork.h"
#include "random.h"
#include "edge.h"
#include "degreepreferencefunction.h"

//This is used by one of the methods internally
#include <vector>
#include <map>

namespace Starsky {

class PrefAtFunction 
{

    public:      
	
	static Node* findPartnerFor(Node* start,
			const Network& net,
			Random& r,
			const DegreePreferenceFunction& dp);
	
	static Node* findInPartnerFor(Node* start,
			const DirectedNetwork& net,
			Random& r,
			const DegreePreferenceFunction& dp);
};

}

#endif
