#ifndef starsky__gnutellacontent
#define starsky__gnutellacontent

#include "contentnode.h"
#include <string>
#include <set>

namespace Starsky {

/**
 * Represents content in gnutella content crawls.  Used for looking
 * at Gnutella in the content space.
 */

class GnutellaContent : public ContentNode {

    public:
        GnutellaContent(const std::string& sha1, const std::string& filename, int size);
	/**
	 * Some files in gnutella might have the same sha1 hash and size, but different names.
	 * @param filename the filename to add for this content
	 */
	void addFileName(const std::string& filename);
	
	const std::set<std::string>& getFileNameSet() const;
	int getSize() const;
        const std::string& getSha1() const;

    protected:
	std::string _sha1;
	std::set<std::string> _filename_set;
	int _size;
};

}

#endif
