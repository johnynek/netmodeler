#include "gnutellacontent.h"

using namespace Starsky;
using namespace std;

GnutellaContent::GnutellaContent(const std::string& sha1,
		                 const std::string& filename,
				 int size) : _sha1(sha1), _size(size)
{
    _filename_set.insert(filename);
}

void GnutellaContent::addFileName(const std::string& filename) {
  _filename_set.insert(filename);
}

const std::set<std::string>& GnutellaContent::getFileNameSet() const {
  return _filename_set;
}

int GnutellaContent::getSize() const {
  return _size;
}

const std::string& GnutellaContent::getSha1() const {
  return _sha1;
}
