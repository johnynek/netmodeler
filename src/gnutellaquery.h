#ifndef starsky__gnutellaquery
#define starsky__gnutellaquery

#include "node.h"
#include <string>

namespace Starsky {

/**
 * Models a query on the gnutella system
 */
	
class GnutellaQuery : public Node {

    public:
	/**
	 * @param mid the Message ID for this query.
	 * Each query should have a globally unique message id.
	 * QueryHit messages for this query should have the
	 * same id.
	 */
        GnutellaQuery(const std::string& mid);

	/**
	 * @return the mid
	 */
	std::string toString() const { return _mid; }
   protected:
	std::string _mid;
	
};
	
}

#endif
